#include "btk_input.h"
#include "btk_ctx.h"
#include "btk_utils.h"

void btk_input_init(btk_input* input) {
    for (int i = 0; i < BTK_ACTION_COUNT * 2; ++i) {
        input->buffer[i] = false;
    }
    input->prv = &input->buffer[0];
    input->cur = &input->buffer[BTK_ACTION_COUNT];
}

void btk_input_update(btk_input* input) {
    const btk_host* host = btk_ctx_host();
    BTK_SWAP(bool*, input->cur, input->prv);
    for (int i = 0; i < BTK_ACTION_COUNT; ++i) {
        input->cur[i] = host->is_action_active(host->ctx, i);
    }
}

bool btk_input_active(const btk_input* input, btk_action action) {
    return input->cur[action];
}

bool btk_input_just_active(const btk_input* input, btk_action action) {
    return input->cur[action] && !input->prv[action];
}

bool btk_input_just_inactive(const btk_input* input, btk_action action) {
    return !input->cur[action] && input->prv[action];
}