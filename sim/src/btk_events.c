#include "btk_events.h"

void btk_events_init(btk_ctx* ctx, btk_events* events) {
    *events = (btk_events){0};
}

void btk_events_send_player_collided_level(btk_ctx* ctx, btk_events* events, btk_event_player_collided_level* player_collided_level) {
    events->sent_player_collided_level = true;
    events->player_collided_level = *player_collided_level;
}