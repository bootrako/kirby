#include "btk_player.h"
#include "btk_events.h"
#include "btk_input.h"
#include "btk_level.h"

#define BTK_PLAYER_BASE_W 16
#define BTK_PLAYER_BASE_H 16
#define BTK_PLAYER_CROUCH_W 16
#define BTK_PLAYER_CROUCH_H 8

void btk_player_init(btk_ctx* ctx, btk_player* player, btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->xform = (btk_rect){ .x = 0.0f, .y = 0.0f, .w = BTK_PLAYER_BASE_W, .h = BTK_PLAYER_BASE_H };
    player->vel = (btk_vec){ .x = 0.0f, .y = 0.0f };
    player->jump_timer = 0.0f;
    player->is_jumping = false;
    player->is_grounded = false;
    player->is_crouching = false;
}

static void btk_player_set_xform_w_h(btk_ctx* ctx, btk_player* player, float w, float h) {
    player->xform.x += player->xform.w - w;
    player->xform.y += player->xform.h - h;
    player->xform.w = w;
    player->xform.h = h;
}

void btk_player_update(btk_ctx* ctx, btk_player* player) {
    btk_vec accel = (btk_vec){ .x = 0.0f, .y = 0.0f };

    bool can_move = !player->is_crouching;
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_MOVE_LEFT) && can_move) {
        accel.x -= ctx->cfg.player_accel.x * BTK_DT;
    }
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_MOVE_RIGHT) && can_move) {
        accel.x += ctx->cfg.player_accel.x * BTK_DT;
    }

    bool can_start_jumping = player->is_grounded && !player->is_crouching;
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

    bool can_start_crouching = player->is_grounded;
    if (btk_input_is_pressed(ctx, player->input, BTK_ACTION_CROUCH) && can_start_crouching) {
        player->is_crouching = true;
        btk_player_set_xform_w_h(ctx, player, BTK_PLAYER_CROUCH_W, BTK_PLAYER_CROUCH_H);
        accel = (btk_vec){ .x = 0.0f, .y = 0.0f };
    }

    bool can_stop_crouching = player->is_crouching;
    if ((btk_input_is_released(ctx, player->input, BTK_ACTION_CROUCH) || btk_input_is_pressed(ctx, player->input, BTK_ACTION_FLOAT)) && can_stop_crouching) {
        player->is_crouching = false;
        btk_player_set_xform_w_h(ctx, player, BTK_PLAYER_BASE_W, BTK_PLAYER_BASE_H);
    }

    player->vel = btk_vec_add(player->vel, accel);

    if (accel.x == 0.0f) {
        float vel_damp = ctx->cfg.player_vel_damp_x * BTK_DT;
        if (player->vel.x > -vel_damp && player->vel.x < vel_damp) {
            player->vel.x = 0.0f;
        } else {
            player->vel.x += vel_damp * -btk_signf(player->vel.x);
        }
    }
    if (!player->is_jumping) {
        player->vel.y += ctx->cfg.player_gravity * BTK_DT;
    }
    player->vel.x = btk_clampf(player->vel.x, ctx->cfg.player_vel_min.x * BTK_DT, ctx->cfg.player_vel_max.x * BTK_DT);
    player->vel.y = btk_clampf(player->vel.y, ctx->cfg.player_vel_min.y * BTK_DT, ctx->cfg.player_vel_max.y * BTK_DT);

    btk_vec desired = btk_vec_add((btk_vec){ .x = player->xform.x, .y = player->xform.y }, player->vel);
    btk_level_collision collision = btk_level_collide(ctx, player->level, player->xform, desired);
    player->xform.x = collision.pos.x;
    player->xform.y = collision.pos.y;

    if (collision.did_collide) {
        btk_event_player_collided_level player_collided_level;
        player_collided_level.vel = player->vel;
        player_collided_level.pos = collision.pos;
        player_collided_level.normal = collision.normal;
        btk_events_send_player_collided_level(ctx, ctx->events, &player_collided_level);    
    }

    if (collision.did_collide && collision.normal.x != 0.0f) {
        player->vel.x = 0.0f;
    }

    bool was_grounded = player->is_grounded;
    player->is_grounded = collision.did_collide && collision.normal.y == -1.0f;
    if (player->is_grounded && !was_grounded) {
        player->jump_timer = 0.0f;
    }
}

void btk_player_start_level(btk_ctx* ctx, btk_player* player, btk_level* level) {
    player->level = level;
    player->xform.x = level->player_start_x;
    player->xform.y = level->player_start_y;
}