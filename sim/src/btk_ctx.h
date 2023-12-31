#ifndef BTK_CTX_H
#define BTK_CTX_H

#include "btk_core.h"

typedef struct btk_events_t btk_events;

typedef struct btk_ctx_t {
    btk_host host;
    btk_cfg cfg;
    btk_events* events;
} btk_ctx;

void btk_ctx_init(btk_ctx* ctx, btk_host host);
void btk_ctx_deinit(btk_ctx* ctx);
void btk_ctx_update(btk_ctx* ctx);

#endif // BTK_CTX_H