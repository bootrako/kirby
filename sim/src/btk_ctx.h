#ifndef BTK_CTX_H
#define BTK_CTX_H

#include <btk_host.h>

void btk_ctx_init(const btk_host* host);
const btk_host* btk_ctx_host();

#endif // BTK_CTX_H