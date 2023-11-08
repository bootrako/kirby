#ifndef BTK_HOST_H
#define BTK_HOST_H

#include "btk_action.h"
#include "btk_data.h"
#include <stdbool.h>

typedef struct btk_host_t {
    void*(*alloc)(void* ctx, int size);                                 // allocate memory with the given size
    void(*free)(void* ctx, void* ptr);                                  // free allocated memory
    void(*panic)(void* ctx, const char* err_msg);                       // crash with an error message
    void(*log)(void* ctx, const char* msg);                             // prints message to output
    bool(*is_action_active)(void* ctx, btk_action action);              // returns true if the action is currently being activated
    const char*(*read_data)(void* ctx, btk_data data, int* out_len);    // opens the specified data file and returns the contents
    void* ctx;                                                          // context object for storing host data
} btk_host;

#endif // BTK_HOST_H