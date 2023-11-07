#include "btk_player.h"
#include "btk_input.h"
#include "btk_level.h"
#include <stddef.h>

void btk_player_init(btk_player* player, const btk_input* input) {
    player->input = input;
    player->level = NULL;
    player->pos = (btk_vec2){ .x = 0, .y = 0 };
}

void btk_player_update(btk_player* player) {
    if (btk_input_action_active(player->input, BTK_SIM_ACTION_MOVE_LEFT)) {
        player->pos.x -= 1;
    }
    if (btk_input_action_active(player->input, BTK_SIM_ACTION_MOVE_RIGHT)) {
        player->pos.x += 1;
    }
    if (btk_input_action_active(player->input, BTK_SIM_ACTION_MOVE_UP)) {
        player->pos.y -= 1;
    }
    if (btk_input_action_active(player->input, BTK_SIM_ACTION_MOVE_DOWN)) {
        player->pos.y += 1;
    }
}