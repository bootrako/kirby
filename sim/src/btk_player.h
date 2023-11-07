#ifndef BTK_PLAYER_H
#define BTK_PLAYER_H

#include "btk_math.h"

typedef struct btk_input_t btk_input;

typedef struct btk_player_t {
    btk_vec2 pos;
} btk_player;

void btk_player_init(btk_player* player);
void btk_player_update(btk_player* player, btk_input* input);

#endif // BTK_PLAYER_H