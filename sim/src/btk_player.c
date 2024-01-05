#include "btk_player.h"
#include "btk_events.h"
#include "btk_input.h"
#include "btk_level.h"

#define BTK_PLAYER_SMALL_SIZE (btk_vec){ .x = 16, .y = 16 }
#define BTK_PLAYER_CROUCH_SIZE (btk_vec){ .x = 16, .y = 8 }

void btk_player_init(btk_ctx* ctx, btk_player* player, btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->xform = (btk_rect){ .pos = BTK_VEC_ZERO, .size = BTK_PLAYER_SMALL_SIZE };
    player->vel = BTK_VEC_ZERO;
    player->jump_timer = 0.0f;
    player->fall_timer = 0.0f;
    player->is_jumping = false;
    player->is_grounded = false;
    player->is_crouching = false;
    player->is_dive_stunned = false;
}

static void btk_player_set_xform_size(btk_ctx* ctx, btk_player* player, btk_vec size) {
    player->xform.pos = btk_vec_add(player->xform.pos, btk_vec_sub(player->xform.size, size));
    player->xform.size = size;
}

void btk_player_update(btk_ctx* ctx, btk_player* player) {
    btk_vec accel = BTK_VEC_ZERO;

    bool can_move = !player->is_crouching && !player->is_dive_stunned;
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_MOVE_LEFT) && can_move) {
        accel.x -= ctx->cfg.player_accel.x * BTK_DT;
    }
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_MOVE_RIGHT) && can_move) {
        accel.x += ctx->cfg.player_accel.x * BTK_DT;
    }

    bool can_start_jumping = player->is_grounded && !player->is_crouching && !player->is_dive_stunned;
    if (btk_input_just_pressed(ctx, player->input, BTK_ACTION_JUMP) && can_start_jumping) {
        player->is_jumping = true;
    }

    bool can_keep_jumping = player->is_jumping;
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_JUMP) && can_keep_jumping) {
        accel.y -= ctx->cfg.player_accel.y * BTK_DT;
        player->jump_timer += BTK_DT;
    }

    bool can_stop_jumping = player->is_jumping;
    if ((btk_input_just_released(ctx, player->input, BTK_ACTION_JUMP) || player->jump_timer > ctx->cfg.player_max_jump_timer) && can_stop_jumping) {
        accel.y = 0.0f;
        player->is_jumping = false;
        player->vel.y = ctx->cfg.player_jump_release_vel_y * BTK_DT;
    }

    bool can_start_crouching = player->is_grounded && !player->is_dive_stunned;
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_CROUCH) && can_start_crouching) {
        player->is_crouching = true;
        btk_player_set_xform_size(ctx, player, BTK_PLAYER_CROUCH_SIZE);
        accel = BTK_VEC_ZERO;
    }

    bool can_stop_crouching = player->is_crouching;
    if ((btk_input_is_released(ctx, player->input, BTK_ACTION_CROUCH) || btk_input_is_pressed(ctx, player->input, BTK_ACTION_FLOAT)) && can_stop_crouching) {
        player->is_crouching = false;
        btk_player_set_xform_size(ctx, player, BTK_PLAYER_SMALL_SIZE);
    }

    player->vel = btk_vec_add(player->vel, accel);

    if (accel.x == 0.0f && !player->is_dive_stunned) {
        float vel_damp = ctx->cfg.player_vel_damp_x * BTK_DT;
        if (player->vel.x > -vel_damp && player->vel.x < vel_damp) {
            player->vel.x = 0.0f;
        } else {
            player->vel.x += vel_damp * -btk_signf(player->vel.x);
        }
    }
    if (!player->is_jumping) {
        player->vel.y += (player->is_dive_stunned ? ctx->cfg.player_gravity_dive_stunned : ctx->cfg.player_gravity) * BTK_DT;
    }
    player->vel.x = btk_clampf(player->vel.x, ctx->cfg.player_vel_min.x * BTK_DT, ctx->cfg.player_vel_max.x * BTK_DT);
    player->vel.y = btk_clampf(player->vel.y, ctx->cfg.player_vel_min.y * BTK_DT, ctx->cfg.player_vel_max.y * BTK_DT);

    btk_vec desired = btk_vec_add(player->xform.pos, player->vel);
    btk_level_collision collision = btk_level_collide(ctx, player->level, player->xform, desired);
    player->xform.pos = collision.pos;

    if (collision.did_collide) {
        btk_event_player_collided_level player_collided_level;
        player_collided_level.vel = player->vel;
        player_collided_level.pos = collision.pos;
        player_collided_level.normal = collision.normal;
        btk_events_send_player_collided_level(ctx, ctx->events, &player_collided_level);    
    }

    if (collision.did_collide) {
        if (collision.normal.x != 0.0f) {
            player->vel.x = 0.0f;
        }
        if (collision.normal.y != 0.0f) {
            player->vel.y = 0.0f;
            player->is_jumping = false;
        }
    }

    bool was_grounded = player->is_grounded;
    player->is_grounded = collision.did_collide && collision.normal.y == -1.0f;
    if (player->is_grounded && !was_grounded) {
        if (player->is_dive_stunned) {
            player->is_dive_stunned = false;
        } else if (player->fall_timer > ctx->cfg.player_fall_dive_timer) {
            player->vel.y = ctx->cfg.player_dive_bounce_vel_y * BTK_DT;
            player->is_dive_stunned = true;
        }

        player->jump_timer = 0.0f;
        player->fall_timer = 0.0f;
    }

    bool is_falling = !player->is_grounded && !player->is_jumping;
    if (is_falling) {
        player->fall_timer += BTK_DT;
    }
}

void btk_player_start_level(btk_ctx* ctx, btk_player* player, btk_level* level) {
    player->level = level;
    player->xform.pos = level->player_start_pos;
}