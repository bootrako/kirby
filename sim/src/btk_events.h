#ifndef BTK_EVENTS_H
#define BTK_EVENTS_H

#include "btk_core.h"

typedef struct btk_events_t {
    btk_event_player_collided_level player_collided_level;
    bool sent_player_collided_level;
} btk_events;

void btk_events_init(btk_ctx* ctx, btk_events* events);

void btk_events_send_player_collided_level(btk_ctx* ctx, btk_events* events, btk_event_player_collided_level* player_collided_level);

#endif // BTK_EVENTS_H