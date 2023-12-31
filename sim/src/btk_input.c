#include "btk_input.h"

void btk_input_init(btk_ctx* ctx, btk_input* input) {
    for (int i = 0; i < BTK_ACTION_COUNT * 2; ++i) {
        input->buffer[i] = false;
    }
    input->prv = &input->buffer[0];
    input->cur = &input->buffer[BTK_ACTION_COUNT];
}

void btk_input_update(btk_ctx* ctx, btk_input* input) {
    BTK_SWAP(bool*, input->cur, input->prv);
    for (int i = 0; i < BTK_ACTION_COUNT; ++i) {
        input->cur[i] = ctx->host.is_action_pressed(ctx->host.ctx, i);
    }
}

bool btk_input_is_pressed(btk_ctx* ctx, btk_input* input, btk_action action) {
    return input->cur[action];
}

bool btk_input_just_pressed(btk_ctx* ctx, btk_input* input, btk_action action) {
    return input->cur[action] && !input->prv[action];
}

bool btk_input_just_released(btk_ctx* ctx, btk_input* input, btk_action action) {
    return !input->cur[action] && input->prv[action];
}