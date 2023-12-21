#include "btk_level.h"
#include "btk_player.h"
#include "btk_read.h"

#define BTK_LEVEL_TILE_WIDTH 16
#define BTK_LEVEL_TILE_HEIGHT 16

void btk_level_init(btk_ctx* ctx, btk_level* level) {
    level->width = 0;
    level->height = 0;
    level->collision = NULL;
}

void btk_level_deinit(btk_ctx* ctx, btk_level* level) {
    if (level->collision != NULL) {
        btk_ctx_free(ctx, level->collision);
        level->collision = NULL;
    }
}

void btk_level_load(btk_ctx* ctx, btk_level* level, btk_data data, btk_player* player) {
    btk_level_deinit(ctx, level);

    int level_data_len = 0;
    char* level_data_str = btk_ctx_read_data(ctx, data, &level_data_len);
    if (level_data_str == NULL) {
        btk_ctx_panic(ctx, "failed to read level data!");
    }

    btk_read read;
    btk_read_init(ctx, &read, level_data_str, level_data_len);

    level->width = btk_read_int(ctx, &read);
    level->height = btk_read_int(ctx, &read);

    level->collision = btk_ctx_alloc(ctx, sizeof(bool) * level->width * level->height);
    if (level->collision == NULL) {
        btk_ctx_panic(ctx, "failed to alloc level collision data!");
    }

    for (int y = 0; y < level->height; ++y) {
        int line_len = 0;
        char* line = btk_read_line(ctx, &read, &line_len);

        if (line_len != level->width) {
            btk_ctx_panic(ctx, "level collision data width does not match level width!");
        }

        for (int x = 0; x < level->width; ++x) {
            if (line[x] != '0' && line[x] != '1') {
                btk_ctx_panic(ctx, "level collision data must be 0 or 1!");
            }
            level->collision[y * level->width + x] = line[x] == '1';
        }
    }

    int player_start_x = btk_read_int(ctx, &read);
    int player_start_y = btk_read_int(ctx, &read);

    player->level = level;
    player->xform.x = (float)(player_start_x * BTK_LEVEL_TILE_WIDTH);
    player->xform.y = (float)(player_start_y * BTK_LEVEL_TILE_HEIGHT);
}

static bool btk_level_collide_x(btk_ctx* ctx, btk_level* level, btk_rect xform, float desired_x, float* out_resolved_x) {
    bool moving_r = desired_x > xform.x;
    int dir_x = moving_r ? 1 : -1;
    int edge_offset_x = moving_r ? (int)(xform.w - 1) : 0;
    int cell_offset_x = moving_r ? (int)(-xform.w) : BTK_LEVEL_TILE_WIDTH;

    int start_cell_x = (int)(xform.x + edge_offset_x) / BTK_LEVEL_TILE_WIDTH;
    int end_cell_x = ((int)(desired_x + edge_offset_x) / BTK_LEVEL_TILE_WIDTH) + dir_x;
    int start_cell_y = (int)(xform.y) / BTK_LEVEL_TILE_HEIGHT;
    int end_cell_y = (int)(xform.y + xform.h - 1) / BTK_LEVEL_TILE_HEIGHT;

    start_cell_x = BTK_CLAMP(start_cell_x, 0, level->width - 1);
    end_cell_x = BTK_CLAMP(end_cell_x, -1, level->width);
    start_cell_y = BTK_CLAMP(start_cell_y, 0, level->height - 1);
    end_cell_y = BTK_CLAMP(end_cell_y, 0, level->height - 1);

    for (int cell_x = start_cell_x; cell_x != end_cell_x; cell_x += dir_x) {
        for (int cell_y = start_cell_y; cell_y <= end_cell_y; ++cell_y) {
            if (level->collision[level->width * cell_y + cell_x]) {
                *out_resolved_x = (float)(cell_x * BTK_LEVEL_TILE_WIDTH + cell_offset_x);
                return true;
            }
        }
    }

    int max_x = (level->width - 1) * BTK_LEVEL_TILE_WIDTH;
    *out_resolved_x = BTK_CLAMP(desired_x, 0, max_x);
    return desired_x >= 0 && desired_x <= max_x;
}

static bool btk_level_collide_y(btk_ctx* ctx, btk_level* level, btk_rect xform, float desired_y, float* out_resolved_y) {
    bool moving_d = desired_y > xform.y;
    int dir_y = moving_d ? 1 : -1;
    int edge_offset_y = moving_d ? (int)(xform.h - 1) : 0;
    int cell_offset_y = moving_d ? (int)(-xform.h) : BTK_LEVEL_TILE_HEIGHT;

    int start_cell_y = (int)(xform.y + edge_offset_y) / BTK_LEVEL_TILE_HEIGHT;
    int end_cell_y = ((int)(desired_y + edge_offset_y) / BTK_LEVEL_TILE_HEIGHT) + dir_y;
    int start_cell_x = (int)(xform.x) / BTK_LEVEL_TILE_WIDTH;
    int end_cell_x = (int)(xform.x + xform.w - 1) / BTK_LEVEL_TILE_WIDTH;

    start_cell_y = BTK_CLAMP(start_cell_y, 0, level->height - 1);
    end_cell_y = BTK_CLAMP(end_cell_y, -1, level->height);
    start_cell_x = BTK_CLAMP(start_cell_x, 0, level->width - 1);
    end_cell_x = BTK_CLAMP(end_cell_x, 0, level->width - 1);

    for (int cell_y = start_cell_y; cell_y != end_cell_y; cell_y += dir_y) {
        for (int cell_x = start_cell_x; cell_x <= end_cell_x; ++cell_x) {
            if (level->collision[level->width * cell_y + cell_x]) {
                *out_resolved_y = (float)(cell_y * BTK_LEVEL_TILE_HEIGHT + cell_offset_y);
                return true;
            }
        }
    }

    int max_y = (level->height - 1) * BTK_LEVEL_TILE_HEIGHT;
    *out_resolved_y = BTK_CLAMP(desired_y, 0, max_y);
    return desired_y >= 0 && desired_y <= max_y;
}

btk_level_collision btk_level_collide(btk_ctx* ctx, btk_level* level, btk_rect xform, btk_vec desired) {
    btk_level_collision collision = { .pos = desired, .collided = false };
    if (desired.x != xform.x) {
        collision.collided |= btk_level_collide_x(ctx, level, xform, desired.x, &collision.pos.x);
    }
    if (desired.y != xform.y) {
        collision.collided |= btk_level_collide_y(ctx, level, xform, desired.y, &collision.pos.y);
    }
    return collision;
}