#include "btk_ctx.h"

btk_ctx g_btk_ctx;

void btk_ctx_init(btk_sim_host host) {
    g_btk_ctx.host = host;
}

const btk_sim_host* btk_ctx_host() {
    return &g_btk_ctx.host;
}