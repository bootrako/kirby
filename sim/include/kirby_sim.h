#ifndef KIRBY_SIM_H
#define KIRBY_SIM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
    void*(*alloc)(void* context, int size);                     // allocate memory with the given size
    void(*free)(void* context, void* ptr);                      // free allocated memory
    void* context;                                              // context object for storing host data
} kirby_sim_host;

typedef struct kirby_sim kirby_sim;

// initializes the simulation. internally allocates memory that can only be freed by calling deinit
kirby_sim* kirby_sim_init(kirby_sim_host host);

// deinitialize the simulation. must be called on a sim pointer that was created with init
void kirby_sim_deinit(kirby_sim* sim);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KIRBY_SIM_H