#ifndef BTK_LEVEL_H
#define BTK_LEVEL_H

#include "btk_math.h"

typedef struct btk_player_t btk_player;

typedef struct btk_level_t {
    int width;
    int height;
    bool* collision;
} btk_level;

typedef struct btk_level_collision_t {
    btk_vec pos;
    bool collided;
} btk_level_collision;

void btk_level_init(btk_ctx* ctx, btk_level* level);
void btk_level_deinit(btk_ctx* ctx, btk_level* level);
void btk_level_load(btk_ctx* ctx, btk_level* level, btk_data data, btk_player* player);
btk_level_collision btk_level_collide(btk_ctx* ctx, btk_level* level, btk_rect xform, btk_vec desired);

#endif // BTK_LEVEL_H