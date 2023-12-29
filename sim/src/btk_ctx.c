#include "btk_ctx.h"

void btk_ctx_init(btk_ctx* ctx, btk_host host) {
    ctx->host = host;
    btk_events_init(ctx, &ctx->events);
}

void btk_ctx_update(btk_ctx* ctx) {
    ctx->host.read_cfg(ctx->host.ctx, &ctx->cfg);
    btk_events_init(ctx, &ctx->events);
}
