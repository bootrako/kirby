#include "btk_player.h"
#include "btk_input.h"
#include "btk_level.h"

static const int btk_player_base_w = 16;
static const int btk_player_base_h = 16;

void btk_player_init(btk_player* player, const btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->xform = (btk_rect){ .x = 0, .y = 0, .w = btk_player_base_w, .h = btk_player_base_h };
}

void btk_player_update(btk_player* player) {
    btk_vec2 desired = (btk_vec2){ .x = player->xform.x, .y = player->xform.y };
    if (btk_input_active(player->input, BTK_ACTION_MOVE_LEFT)) {
        desired.x -= 1;
    }
    if (btk_input_active(player->input, BTK_ACTION_MOVE_RIGHT)) {
        desired.x += 1;
    }
    if (btk_input_active(player->input, BTK_ACTION_MOVE_UP)) {
        desired.y -= 1;
    }
    if (btk_input_active(player->input, BTK_ACTION_MOVE_DOWN)) {
        desired.y += 1;
    }

    btk_vec2 actual = btk_level_move(player->level, player->xform, desired);
    player->xform.x = actual.x;
    player->xform.y = actual.y;
}