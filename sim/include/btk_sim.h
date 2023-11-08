#ifndef BTK_SIM_H
#define BTK_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "btk_host.h"

typedef struct btk_sim_t btk_sim;

// initializes the simulation. internally allocates memory that can only be freed by calling deinit
btk_sim* btk_sim_init(const btk_host* host);

// deinitialize the simulation. must be called on a sim pointer that was created with init
void btk_sim_deinit(btk_sim* sim);

// performs sim frame updates based on the time passed since last update
void btk_sim_update(btk_sim* sim, float delta_time);

// gets the player's current position, in world space.
void btk_sim_get_player_pos(const btk_sim* sim, int* out_x, int* out_y);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BTK_SIM_H