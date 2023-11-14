#ifndef BTK_LEVEL_H
#define BTK_LEVEL_H

#include <btk_data.h>
#include "btk_math.h"

typedef struct btk_player_t btk_player;

typedef struct btk_level_t {
    int width;
    int height;
    bool* collision;
} btk_level;

void btk_level_init(btk_level* level);
void btk_level_deinit(btk_level* level);
void btk_level_load(btk_level* level, btk_data data, btk_player* player);
btk_vec2 btk_level_move(const btk_level* level, btk_rect xform, btk_vec2 desired);

#endif // BTK_LEVEL_H