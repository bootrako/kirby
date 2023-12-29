#include "btk_ctx.h"
#include "btk_events.h"

void btk_events_init(btk_ctx* ctx, btk_events* events) {
    events->player_landed_count = 0;
}

void btk_events_merge(btk_ctx* ctx, btk_events* dst, btk_events* src) {
    dst->player_landed_count += src->player_landed_count;
}

void btk_events_send_player_landed(btk_ctx* ctx, btk_events* events) {
    events->player_landed_count += 1;
}