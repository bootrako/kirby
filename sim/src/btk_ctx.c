#include "btk_ctx.h"

typedef struct btk_ctx_t {
    btk_host host;
} btk_ctx;

btk_ctx g_btk_ctx;

void btk_ctx_init(const btk_host* host) {
    g_btk_ctx.host = *host;
}

const btk_host* btk_ctx_host() {
    return &g_btk_ctx.host;
}