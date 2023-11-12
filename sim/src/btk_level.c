#include "btk_level.h"
#include "btk_ctx.h"
#include "btk_read.h"
#include "btk_player.h"
#include "btk_utils.h"
#include <stddef.h>

const static int tile_width = 16;
const static int tile_height = 16;

void btk_level_init(btk_level* level) {
    level->width = 0;
    level->height = 0;
    level->collision = NULL;
}

void btk_level_deinit(btk_level* level) {
    const btk_host* host = btk_ctx_host();
    if (level->collision != NULL) {
        host->free(host->ctx, level->collision);
        level->collision = NULL;
    }
}

void btk_level_load(btk_level* level, btk_data data, btk_player* player) {
    btk_level_deinit(level);

    const btk_host* host = btk_ctx_host();
    
    int level_data_len = 0;
    const char* level_data_str = host->read_data(host->ctx, data, &level_data_len);
    if (level_data_str == NULL) {
        host->panic(host->ctx, "failed to read level data!");
    }

    btk_read read;
    btk_read_init(&read, level_data_str, level_data_len);

    btk_read_int(&read, &level->width);
    btk_read_int(&read, &level->height);

    level->collision = host->alloc(host->ctx, sizeof(bool) * level->width * level->height);
    if (level->collision == NULL) {
        host->panic(host->ctx, "failed to alloc level collision data!");
    }

    for (int y = 0; y < level->height; ++y) {
        const char* line;
        int line_len;
        btk_read_line(&read, &line, &line_len);

        if (line_len != level->width) {
            host->panic(host->ctx, "level collision data width does not match level width!");
        }

        for (int x = 0; x < level->width; ++x) {
            if (line[x] != '0' && line[x] != '1') {
                host->panic(host->ctx, "level collision data must be 0 or 1");
            }
            level->collision[y * level->height + x] = line[x] == '1';
        }
    }

    int player_start_x = 0;
    int player_start_y = 0;
    btk_read_int(&read, &player_start_x);
    btk_read_int(&read, &player_start_y);

    player->level = level;
    player->pos.x = player_start_x * tile_width;
    player->pos.y = player_start_y * tile_height;
}