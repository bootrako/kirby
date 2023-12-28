#ifndef BTK_PLAYER_H
#define BTK_PLAYER_H

#include "btk_math.h"

typedef struct btk_input_t btk_input;
typedef struct btk_level_t btk_level;

typedef struct btk_player_t {
    btk_input* input;
    btk_level* level;
    btk_rect xform;
    btk_vec vel;
    float jump_timer;
    bool is_jumping;
    bool is_grounded;
} btk_player;

void btk_player_init(btk_ctx* ctx, btk_player* player, btk_input* input);
void btk_player_update(btk_ctx* ctx, btk_player* player);
void btk_player_start_level(btk_ctx* ctx, btk_player* player, btk_level* level);

#endif // BTK_PLAYER_H