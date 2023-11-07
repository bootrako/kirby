#include "btk_player.h"
#include "btk_input.h"

void btk_player_init(btk_player* player) {
    player->pos = (btk_vec2){ .x = 0, .y = 0 };
}

void btk_player_update(btk_player* player, btk_input* input) {
    if (btk_input_action_active(input, BTK_SIM_ACTION_MOVE_LEFT)) {
        player->pos.x -= 1;
    }
    if (btk_input_action_active(input, BTK_SIM_ACTION_MOVE_RIGHT)) {
        player->pos.x += 1;
    }
    if (btk_input_action_active(input, BTK_SIM_ACTION_MOVE_UP)) {
        player->pos.y -= 1;
    }
    if (btk_input_action_active(input, BTK_SIM_ACTION_MOVE_DOWN)) {
        player->pos.y += 1;
    }
}