#include "btk_player.h"
#include "btk_input.h"
#include "btk_level.h"

#define BTK_PLAYER_BASE_W 16
#define BTK_PLAYER_BASE_H 16

void btk_player_init(btk_ctx* ctx, btk_player* player, btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->xform = (btk_rect){ .x = 0, .y = 0, .w = BTK_PLAYER_BASE_W, .h = BTK_PLAYER_BASE_H };
}

void btk_player_update(btk_ctx* ctx, btk_player* player) {
    btk_vec2 desired = (btk_vec2){ .x = player->xform.x, .y = player->xform.y };
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_LEFT)) {
        desired.x -= 1;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_RIGHT)) {
        desired.x += 1;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_UP)) {
        desired.y -= 1;
    }
    if (btk_input_active(ctx, player->input, BTK_ACTION_MOVE_DOWN)) {
        desired.y += 1;
    }

    btk_vec2 actual = btk_level_collide(ctx, player->level, player->xform, desired);
    player->xform.x = actual.x;
    player->xform.y = actual.y;
}