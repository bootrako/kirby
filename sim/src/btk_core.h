#ifndef BTK_CORE_H
#define BTK_CORE_H

#include <stddef.h>

#include <btk_sim.h>
#include "btk_ctx.h"

#define BTK_DT (1.0f / 60.0f)

#define BTK_SWAP(T, a, b) { T swap_tmp = a; a = b; b = swap_tmp; }
#define BTK_MIN(a, b) ((a) > (b) ? b : a)
#define BTK_MAX(a, b) ((a) > (b) ? a : b)
#define BTK_CLAMP(x, min, max) BTK_MAX(min, BTK_MIN(max, x))

#ifndef NDEBUG
#define BTK_ASSERT(ctx, expr) { if (!(expr)) btk_ctx_panic(ctx, "assertion failed: " #expr); }
#else
#define BTK_ASSERT(ctx, expr)
#endif

#endif // BTK_CORE_H