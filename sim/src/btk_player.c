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
}

void btk_player_update(btk_ctx* ctx, btk_player* player) {
    btk_vec accel = (btk_vec){ .x = 0, .y = 0 };
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_LEFT)) {
        accel.x -= ctx->cfg.player_accel[0] * BTK_DT;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_RIGHT)) {
        accel.x += ctx->cfg.player_accel[0] * BTK_DT;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_UP)) {
        accel.y -= ctx->cfg.player_accel[1] * BTK_DT;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_DOWN)) {
        accel.y += ctx->cfg.player_accel[1] * BTK_DT;
    }

    player->vel = btk_vec_mul(player->vel, (btk_vec){ .x = ctx->cfg.player_vel_damp[0], .y = ctx->cfg.player_vel_damp[1] });
    player->vel = btk_vec_add(player->vel, accel);
    player->vel.x = btk_clampf(player->vel.x, -ctx->cfg.player_vel_max[0] * BTK_DT, ctx->cfg.player_vel_max[0] * BTK_DT);
    player->vel.y = btk_clampf(player->vel.y, -ctx->cfg.player_vel_max[1] * BTK_DT, ctx->cfg.player_vel_max[1] * BTK_DT);

    btk_vec desired = btk_vec_add((btk_vec){ .x = player->xform.x, .y = player->xform.y }, player->vel);
    btk_level_collision collision = btk_level_collide(ctx, player->level, player->xform, desired);
    player->xform.x = collision.pos.x;
    player->xform.y = collision.pos.y;
}

void btk_player_start_level(btk_ctx* ctx, btk_player* player, btk_level* level) {
    player->level = level;
    player->xform.x = level->player_start_x;
    player->xform.y = level->player_start_y;
}