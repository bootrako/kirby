#ifndef BTK_SIM_H
#define BTK_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>

typedef enum btk_sim_action_t {
    BTK_SIM_ACTION_MOVE_LEFT,
    BTK_SIM_ACTION_MOVE_RIGHT,
    BTK_SIM_ACTION_MOVE_UP,
    BTK_SIM_ACTION_MOVE_DOWN,
    BTK_SIM_ACTION_COUNT,
} btk_sim_action;

typedef enum btk_sim_data_file_t {
    BTK_SIM_DATA_FILE_GREEN_GREENS,
    BTK_SIM_DATA_FILE_COUNT,
} btk_sim_data_file;

typedef struct btk_sim_data_t {
    const char* contents;
    int len;
} btk_sim_data;

typedef struct btk_sim_host_t {
    void*(*alloc)(void* context, int size);                                 // allocate memory with the given size
    void(*free)(void* context, void* ptr);                                  // free allocated memory
    void(*panic)(void* context, const char* err_msg);                       // crash with an error message
    void(*log)(void* context, const char* msg);                             // prints message to output
    bool(*is_action_active)(void* context, btk_sim_action action);          // returns true if the action is currently being activated
    btk_sim_data(*read_data)(void* context, btk_sim_data_file data_file);   // opens the specified data file and returns the contents
    void* context;                                                          // context object for storing host data
} btk_sim_host;

typedef struct btk_sim btk_sim;

// initializes the simulation. internally allocates memory that can only be freed by calling deinit
btk_sim* btk_sim_init(btk_sim_host host);

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