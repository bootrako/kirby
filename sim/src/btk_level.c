#include "btk_level.h"
#include "btk_ctx.h"
#include "btk_read.h"
#include "btk_player.h"
#include "btk_utils.h"
#include <stddef.h>

void btk_level_init(btk_level* level) {
    level->width = 0;
    level->height = 0;
}

void btk_level_load(btk_level* level, btk_data data, btk_player* player) {
    const btk_host* host = btk_ctx_host();
    
    int level_data_len = 0;
    const char* level_data_str = host->read_data(host->ctx, data, &level_data_len);
    if (level_data_str == NULL) {
        host->panic(host->ctx, "failed to read level data!");
    }

    btk_read read;
    btk_read_init(&read, level_data_str, level_data_len);
    if (!btk_read_int(&read, &level->width)) {
        host->panic(host->ctx, "failed to read level width!");
    }
    if (!btk_read_int(&read, &level->height)) {
        host->panic(host->ctx, "failed to read level height!");
    }

    player->level = level;
}