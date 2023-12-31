#ifndef BTK_SIM_H
#define BTK_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>

typedef struct btk_vec_t {
    float x;
    float y;
} btk_vec;

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

typedef struct btk_cfg_t {
    btk_vec player_accel;
    btk_vec player_vel_min;
    btk_vec player_vel_max;
    float player_vel_damp_x;
    float player_jump_release_vel_y;
    float player_gravity;
    float player_gravity_dive_stunned;
    float player_max_jump_timer;
    float player_fall_dive_timer;
    float player_dive_bounce_vel_y;
} btk_cfg;

typedef struct btk_host_t {
    void*(*alloc)(void* ctx, int size);                                 // allocate memory with the given size
    void(*free)(void* ctx, void* ptr);                                  // free allocated memory
    void(*panic)(void* ctx, char* err_msg);                             // crash with an error message
    void(*log)(void* ctx, char* msg);                                   // prints message to output
    bool(*is_action_pressed)(void* ctx, btk_action action);             // returns true if the action is currently being activated
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

btk_cfg* btk_sim_get_cfg(btk_sim* sim);

btk_vec btk_sim_get_player_pos(btk_sim* sim);

btk_vec btk_sim_get_player_vel(btk_sim* sim);

float btk_sim_get_player_fall_timer(btk_sim* sim);

bool btk_sim_get_player_is_grounded(btk_sim* sim);

bool btk_sim_get_player_is_crouching(btk_sim* sim);

bool btk_sim_get_player_is_dive_stunned(btk_sim* sim);

typedef struct btk_event_player_collided_level_t {
    btk_vec vel;
    btk_vec pos;
    btk_vec normal;
} btk_event_player_collided_level;

bool btk_sim_get_event_player_collided_level(btk_sim* sim, btk_event_player_collided_level** out_event_data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BTK_SIM_H