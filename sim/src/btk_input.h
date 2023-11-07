#ifndef BTK_INPUT_H
#define BTK_INPUT_H

#include <btk_sim.h>

typedef struct btk_input_t {
    bool input_buffer[BTK_SIM_ACTION_COUNT * 2];
    bool* prv_input;
    bool* cur_input;
} btk_input;

void btk_input_init(btk_input* input);
void btk_input_update(btk_input* input);

bool btk_input_action_active(const btk_input* input, btk_sim_action action);
bool btk_input_action_just_activated(const btk_input* input, btk_sim_action action);
bool btk_input_action_just_deactivated(const btk_input* input, btk_sim_action action);

#endif // BTK_INPUT_H