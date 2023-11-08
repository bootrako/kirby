#include "btk_level.h"
#include "btk_ctx.h"
#include "btk_player.h"
#include "btk_utils.h"

void btk_level_init(btk_level* level) {
    level->width = 0;
    level->height = 0;
}

void btk_level_load(btk_level* level, btk_data data, btk_player* player) {
    BTK_REF(data);
    player->level = level;
}