#ifndef BTK_SIM_H
#define BTK_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>

typedef enum btk_action_t {
    BTK_ACTION_MOVE_LEFT,
    BTK_ACTION_MOVE_RIGHT,
    BTK_ACTION_FLOAT,
    BTK_ACTION_CROUCH,
    BTK_ACTION_JUMP,
    BTK_ACTION_INHALE,
    BTK_ACTION_COUNT,
} btk_action;

typedef enum btk_data_t {
    BTK_DATA_GREEN_GREENS,
    BTK_DATA_COUNT,
} btk_data;

typedef struct btk_sim_vec_t {
    float x;
    float y;
} btk_sim_vec;

typedef struct btk_cfg_t {
    btk_sim_vec player_accel;
    btk_sim_vec player_vel_max;
    float player_vel_damp_x;
    float player_gravity;
    float player_max_jump_timer;
} btk_cfg;

typedef struct btk_host_t {
    void*(*alloc)(void* ctx, int size);                                 // allocate memory with the given size
    void(*free)(void* ctx, void* ptr);                                  // free allocated memory
    void(*panic)(void* ctx, char* err_msg);                             // crash with an error message
    void(*log)(void* ctx, char* msg);                                   // prints message to output
    bool(*is_action_active)(void* ctx, btk_action action);              // returns true if the action is currently being activated
    char*(*read_data)(void* ctx, btk_data data, int* out_len);          // opens the specified data file and returns the contents
    void(*read_cfg)(void* ctx, btk_cfg* cfg);                           // reads the current state of dynamic config
    void* ctx;                                                          // context object for storing host data
} btk_host;

typedef struct btk_sim_t btk_sim;

// initializes the simulation. internally allocates memory that can only be freed by calling deinit
btk_sim* btk_sim_init(btk_host host);

// deinitialize the simulation. must be called on a sim pointer that was created with init
void btk_sim_deinit(btk_sim* sim);

// performs sim frame updates based on the time passed since last update
void btk_sim_update(btk_sim* sim, float delta_time);

btk_sim_vec btk_sim_get_player_pos(btk_sim* sim);

btk_sim_vec btk_sim_get_player_vel(btk_sim* sim);

bool btk_sim_get_player_is_grounded(btk_sim* sim);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BTK_SIM_H