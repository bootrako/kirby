#ifndef BTK_LEVEL_H
#define BTK_LEVEL_H

#include <btk_sim.h>

typedef struct btk_player_t btk_player;

typedef struct btk_level_t {
    int width;
    int height;
} btk_level;

void btk_level_init(btk_level* level);
void btk_level_load(btk_level* level, btk_sim_data_file data_file, btk_player* player);

#endif // BTK_LEVEL_H