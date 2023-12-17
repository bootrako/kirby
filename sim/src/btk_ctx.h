#ifndef BTK_CTX_H
#define BTK_CTX_H

#include "btk_core.h"

typedef struct btk_ctx {
    btk_host host;
} btk_ctx;

void btk_ctx_init(btk_ctx* ctx, btk_host host);

void* btk_ctx_alloc(btk_ctx* ctx, int size);
void btk_ctx_free(btk_ctx* ctx, void* ptr);
void btk_ctx_panic(btk_ctx* ctx, char* err_msg);
void btk_ctx_log(btk_ctx* ctx, char* msg);
bool btk_ctx_is_action_active(btk_ctx* ctx, btk_action action);
char* btk_ctx_read_data(btk_ctx* ctx, btk_data data, int* out_len);

#endif // BTK_CTX_H