#ifndef KIRBY_SIM_H
#define KIRBY_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum kirby_sim_input_action_t {
    KIRBY_SIM_INPUT_ACTION_MOVE_LEFT,
    KIRBY_SIM_INPUT_ACTION_MOVE_RIGHT,
    KIRBY_SIM_INPUT_ACTION_MOVE_UP,
    KIRBY_SIM_INPUT_ACTION_MOVE_DOWN,
} kirby_sim_input_action;

typedef struct kirby_sim_host_t {
    void*(*alloc)(void* context, int size);                                             // allocate memory with the given size
    void(*free)(void* context, void* ptr);                                              // free allocated memory
    void(*panic)(void* context, const char* msg);                                       // crash with an error message
    bool(*input_action_pressed)(void* context, kirby_sim_input_action input_action);    // returns true if the input action is currently pressed
    void* context;                                                                      // context object for storing host data
} kirby_sim_host;

typedef struct kirby_sim_pos_t {
    int x;
    int y;
} kirby_sim_pos;

typedef struct kirby_sim kirby_sim;

// initializes the simulation. internally allocates memory that can only be freed by calling deinit
kirby_sim* kirby_sim_init(kirby_sim_host host);

// deinitialize the simulation. must be called on a sim pointer that was created with init
void kirby_sim_deinit(kirby_sim* sim);

// performs frame updates based on the time passed since last update. returns the number of frames processed
int kirby_sim_update(kirby_sim* sim, float delta_time);

// gets the player's current position, in world space.
kirby_sim_pos kirby_sim_get_player_pos(const kirby_sim* sim);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KIRBY_SIM_H