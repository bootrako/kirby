#ifndef BTK_CTX_H
#define BTK_CTX_H

#include <btk_sim.h>

typedef struct btk_ctx_t {
    btk_sim_host host;
} btk_ctx;

void btk_ctx_init(btk_sim_host host);
const btk_sim_host* btk_ctx_host();

#endif // BTK_CTX_H