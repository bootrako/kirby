#ifndef BTK_PLAYER_H
#define BTK_PLAYER_H

#include "btk_math.h"

typedef struct btk_input_t btk_input;
typedef struct btk_level_t btk_level;

typedef struct btk_player_t {
    const btk_input* input;
    const btk_level* level;
    btk_vec2 pos;
} btk_player;

void btk_player_init(btk_player* player, const btk_input* input);
void btk_player_update(btk_player* player);

#endif // BTK_PLAYER_H