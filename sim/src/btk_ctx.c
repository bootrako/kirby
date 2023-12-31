#include "btk_ctx.h"
#include "btk_events.h"

void btk_ctx_init(btk_ctx* ctx, btk_host host) {
    ctx->host = host;
    ctx->events = ctx->host.alloc(ctx->host.ctx, sizeof(btk_events));
    btk_events_init(ctx, ctx->events);
}

void btk_ctx_deinit(btk_ctx* ctx) {
    ctx->host.free(ctx->host.ctx, ctx->events);
}

void btk_ctx_update(btk_ctx* ctx) {
    ctx->host.read_cfg(ctx->host.ctx, &ctx->cfg);
    btk_events_init(ctx, ctx->events);
}
