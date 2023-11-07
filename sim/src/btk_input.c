#include "btk_input.h"
#include "btk_utils.h"

void btk_input_init(btk_input* input, btk_sim_host* host) {
    input->host = host;
    for (int i = 0; i < BTK_SIM_ACTION_COUNT * 2; ++i) {
        input->input_buffer[i] = false;
    }
    input->prv_input = &input->input_buffer[0];
    input->cur_input = &input->input_buffer[BTK_SIM_ACTION_COUNT];
}

void btk_input_update(btk_input* input) {
    BTK_SWAP(bool*, input->cur_input, input->prv_input);
    for (int i = 0; i < BTK_SIM_ACTION_COUNT; ++i) {
        input->cur_input[i] = input->host->is_action_active(input->host->context, i);
    }
}

bool btk_input_action_active(const btk_input* input, btk_sim_action action) {
    return input->cur_input[action];
}

bool btk_input_action_just_activated(const btk_input* input, btk_sim_action action) {
    return input->cur_input[action] && !input->prv_input[action];
}

bool btk_input_action_just_deactivated(const btk_input* input, btk_sim_action action) {
    return !input->cur_input[action] && input->prv_input[action];
}