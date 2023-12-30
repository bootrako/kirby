#ifndef BTK_EVENTS_H
#define BTK_EVENTS_H

#include "btk_core.h"

typedef struct btk_ctx_t btk_ctx;

typedef struct btk_events_t {
    int player_landed_count;
} btk_events;

void btk_events_init(btk_ctx* ctx, btk_events* event);

void btk_events_send_player_landed(btk_ctx* ctx, btk_events* event);

#endif // BTK_EVENTS_H