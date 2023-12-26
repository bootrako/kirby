#include "btk_player.h"
#include "btk_input.h"
#include "btk_level.h"

#define BTK_PLAYER_BASE_W 16
#define BTK_PLAYER_BASE_H 16

void btk_player_init(btk_ctx* ctx, btk_player* player, btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->xform = (btk_rect){ .x = 0.0f, .y = 0.0f, .w = BTK_PLAYER_BASE_W, .h = BTK_PLAYER_BASE_H };
    player->vel = (btk_vec){ .x = 0.0f, .y = 0.0f };
    player->can_jump = false;
    player->jump_timer = 0.0f;
}

void btk_player_update(btk_ctx* ctx, btk_player* player) {
    btk_vec accel = (btk_vec){ .x = 0.0f, .y = 0.0f };
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_LEFT)) {
        accel.x -= ctx->cfg.player_accel[0] * BTK_DT;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_RIGHT)) {
        accel.x += ctx->cfg.player_accel[0] * BTK_DT;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_JUMP) && player->can_jump) {
        accel.y -= ctx->cfg.player_accel[1] * BTK_DT;
        player->jump_timer += BTK_DT;
    }
    if ((btk_input_just_inactive(ctx, player->input, BTK_ACTION_JUMP) && player->can_jump) || player->jump_timer > ctx->cfg.player_max_jump_timer) {
        player->can_jump = false;
    }
    if (!player->can_jump) {
        accel.y += ctx->cfg.player_gravity * BTK_DT;
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
    player->vel.x = btk_clampf(player->vel.x, -ctx->cfg.player_vel_max[0] * BTK_DT, ctx->cfg.player_vel_max[0] * BTK_DT);
    player->vel.y = btk_clampf(player->vel.y, -ctx->cfg.player_vel_max[1] * BTK_DT, ctx->cfg.player_vel_max[1] * BTK_DT);

    btk_vec desired = btk_vec_add((btk_vec){ .x = player->xform.x, .y = player->xform.y }, player->vel);
    btk_level_collision collision = btk_level_collide(ctx, player->level, player->xform, desired);
    player->xform.x = collision.pos.x;
    player->xform.y = collision.pos.y;

    if (!player->can_jump && collision.collided && collision.normal.y == -1.0f) {
        player->can_jump = true;
        player->vel.y = 0.0f;
        player->jump_timer = 0.0f;
    }
}

void btk_player_start_level(btk_ctx* ctx, btk_player* player, btk_level* level) {
    player->level = level;
    player->xform.x = level->player_start_x;
    player->xform.y = level->player_start_y;
}