#include "btk_ctx.h"

void btk_ctx_init(btk_ctx* ctx, btk_host host) {
    ctx->host = host;
}

void btk_ctx_update(btk_ctx* ctx) {
    ctx->host.read_cfg(ctx->host.ctx, &ctx->cfg);
}

void* btk_ctx_alloc(btk_ctx* ctx, int size) {
    return ctx->host.alloc(ctx->host.ctx, size);
}

void btk_ctx_free(btk_ctx* ctx, void* ptr) {
    ctx->host.free(ctx->host.ctx, ptr);
}

void btk_ctx_panic(btk_ctx* ctx, char* err_msg) {
    ctx->host.panic(ctx->host.ctx, err_msg);
}

void btk_ctx_log(btk_ctx* ctx, char* msg) {
    ctx->host.log(ctx->host.ctx, msg);
}

bool btk_ctx_is_action_pressed(btk_ctx* ctx, btk_action action) {
    return ctx->host.is_action_pressed(ctx->host.ctx, action);
}

char* btk_ctx_read_data(btk_ctx* ctx, btk_data data, int* out_len) {
    return ctx->host.read_data(ctx->host.ctx, data, out_len);
}
