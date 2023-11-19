#include "btk_level.h"
#include "btk_ctx.h"
#include "btk_read.h"
#include "btk_player.h"

static const int btk_level_tile_width = 16;
static const int btk_level_tile_height = 16;

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

    level->width = btk_read_int(&read);
    level->height = btk_read_int(&read);

    level->collision = host->alloc(host->ctx, sizeof(bool) * level->width * level->height);
    if (level->collision == NULL) {
        host->panic(host->ctx, "failed to alloc level collision data!");
    }

    for (int y = 0; y < level->height; ++y) {
        int line_len = 0;
        const char* line = btk_read_line(&read, &line_len);

        if (line_len != level->width) {
            host->panic(host->ctx, "level collision data width does not match level width!");
        }

        for (int x = 0; x < level->width; ++x) {
            if (line[x] != '0' && line[x] != '1') {
                host->panic(host->ctx, "level collision data must be 0 or 1!");
            }
            level->collision[y * level->height + x] = line[x] == '1';
        }
    }

    int player_start_x = btk_read_int(&read);
    int player_start_y = btk_read_int(&read);

    player->level = level;
    player->xform.x = player_start_x * btk_level_tile_width;
    player->xform.y = player_start_y * btk_level_tile_height;
}

static int btk_level_collide_x(const btk_level* level, btk_rect xform, int desired_x) {
    bool moving_r = desired_x > xform.x;
    int dir_x = moving_r ? 1 : -1;
    int edge_offset_x = moving_r ? xform.w - 1 : 0;
    int cell_offset_x = moving_r ? -xform.w : btk_level_tile_width;

    int start_cell_x = (xform.x + edge_offset_x) / btk_level_tile_width;
    int end_cell_x = ((desired_x + edge_offset_x) / btk_level_tile_width) + dir_x;
    int start_cell_y = xform.y / btk_level_tile_height;
    int end_cell_y = (xform.y + xform.h - 1) / btk_level_tile_height;

    for (int cell_x = start_cell_x; cell_x != end_cell_x; cell_x += dir_x) {
        for (int cell_y = start_cell_y; cell_y <= end_cell_y; ++cell_y) {
            if (level->collision[level->width * cell_y + cell_x]) {
                return cell_x * btk_level_tile_width + cell_offset_x;
            }
        }
    }

    return desired_x;
}

static int btk_level_collide_y(const btk_level* level, btk_rect xform, int desired_y) {
    bool moving_d = desired_y > xform.y;
    int dir_y = moving_d ? 1 : -1;
    int edge_offset_y = moving_d ? xform.h - 1 : 0;
    int cell_offset_y = moving_d ? -xform.h : btk_level_tile_height;

    int start_cell_y = (xform.y + edge_offset_y) / btk_level_tile_height;
    int end_cell_y = ((desired_y + edge_offset_y) / btk_level_tile_height) + dir_y;
    int start_cell_x = xform.x / btk_level_tile_width;
    int end_cell_x = (xform.x + xform.w - 1) / btk_level_tile_width;

    for (int cell_y = start_cell_y; cell_y != end_cell_y; cell_y += dir_y) {
        for (int cell_x = start_cell_x; cell_x <= end_cell_x; ++cell_x) {
            if (level->collision[level->width * cell_y + cell_x]) {
                return cell_y * btk_level_tile_height + cell_offset_y;
            }
        }
    }

    return desired_y;
}

btk_vec2 btk_level_collide(const btk_level* level, btk_rect xform, btk_vec2 desired) {
    btk_vec2 actual = desired;

    if (desired.x != xform.x) {
        actual.x = btk_level_collide_x(level, xform, desired.x);
    }

    if (desired.y != xform.y) {
        actual.y = btk_level_collide_y(level, xform, desired.y);
    }

    return actual;
}