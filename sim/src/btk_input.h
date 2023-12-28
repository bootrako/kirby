#ifndef BTK_INPUT_H
#define BTK_INPUT_H

#include "btk_core.h"

typedef struct btk_input_t {
    bool buffer[BTK_ACTION_COUNT * 2];
    bool* prv;
    bool* cur;
} btk_input;

void btk_input_init(btk_ctx* ctx, btk_input* input);
void btk_input_update(btk_ctx* ctx, btk_input* input);

bool btk_input_pressed(btk_ctx* ctx, btk_input* input, btk_action action);
bool btk_input_just_pressed(btk_ctx* ctx, btk_input* input, btk_action action);
bool btk_input_just_released(btk_ctx* ctx, btk_input* input, btk_action action);

#endif // BTK_INPUT_H