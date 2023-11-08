#ifndef BTK_INPUT_H
#define BTK_INPUT_H

#include <btk_action.h>
#include <stdbool.h>

typedef struct btk_input_t {
    bool buffer[BTK_ACTION_COUNT * 2];
    bool* prv;
    bool* cur;
} btk_input;

void btk_input_init(btk_input* input);
void btk_input_update(btk_input* input);

bool btk_input_active(const btk_input* input, btk_action action);
bool btk_input_just_active(const btk_input* input, btk_action action);
bool btk_input_just_inactive(const btk_input* input, btk_action action);

#endif // BTK_INPUT_H