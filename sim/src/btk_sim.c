#include <btk_sim.h>
#include "btk_events.h"
#include "btk_input.h"
#include "btk_level.h"
#include "btk_math.h"
#include "btk_player.h"

#define BTK_SIM_MAX_FRAMES_PER_UPDATE 10

struct btk_sim_t {
    btk_ctx ctx;
    btk_input input;
    btk_level level;
    btk_player player;
    btk_events events_since_last_update[BTK_SIM_MAX_FRAMES_PER_UPDATE];
    int frames_since_last_update;
    float frame_accumulator;
};

btk_sim* btk_sim_init(btk_host host) {
    btk_sim* sim = host.alloc(host.ctx, sizeof(btk_sim));

    btk_ctx_init(&sim->ctx, host);
    btk_input_init(&sim->ctx, &sim->input);
    btk_level_init(&sim->ctx, &sim->level);
    btk_player_init(&sim->ctx, &sim->player, &sim->input);
    sim->frame_accumulator = 0.0f;

    btk_level_load(&sim->ctx, &sim->level, BTK_DATA_GREEN_GREENS);
    btk_player_start_level(&sim->ctx, &sim->player, &sim->level);

    return sim;
}

void btk_sim_deinit(btk_sim* sim) {
    btk_level_deinit(&sim->ctx, &sim->level);
    btk_ctx_deinit(&sim->ctx);
    sim->ctx.host.free(sim->ctx.host.ctx, sim);
}

void btk_sim_update(btk_sim* sim, float delta_time) {
    int update_frame = 0;

    sim->frame_accumulator = btk_minf(sim->frame_accumulator + delta_time, BTK_SIM_MAX_FRAMES_PER_UPDATE * BTK_DT);
    while (sim->frame_accumulator > BTK_DT && update_frame < BTK_SIM_MAX_FRAMES_PER_UPDATE) {
        btk_ctx_update(&sim->ctx);
        btk_input_update(&sim->ctx, &sim->input);
        btk_player_update(&sim->ctx, &sim->player); 

        sim->events_since_last_update[update_frame] = *sim->ctx.events;
        sim->frame_accumulator -= BTK_DT;

        update_frame++;
    }

    sim->frames_since_last_update = update_frame;
}

btk_vec btk_sim_get_player_pos(btk_sim* sim) {
    return (btk_vec){ .x = sim->player.xform.x, .y = sim->player.xform.y };
}

btk_vec btk_sim_get_player_vel(btk_sim* sim) {
    return sim->player.vel;
}

bool btk_sim_get_player_is_grounded(btk_sim* sim) {
    return sim->player.is_grounded;
}

#define BTK_SIM_FIND_NEXT_EVENT(sim, ptr, data_field, bool_field) \
    ptrdiff_t index = 0; \
    if (*ptr != NULL) { \
        btk_events* events = (btk_events*)((char*)*ptr - offsetof(btk_events, data_field)); \
        index = (events - sim->events_since_last_update) + 1; \
    } \
    if (index >= 0) { \
        while (index < sim->frames_since_last_update) { \
            if (sim->events_since_last_update[index].bool_field) { \
                *ptr = &sim->events_since_last_update[index].data_field; \
                return true; \
            } \
            index++; \
        } \
    } \
    *ptr = NULL; \
    return false

bool btk_sim_get_event_player_collided_level(btk_sim* sim, btk_event_player_collided_level** out_event_data) {
    BTK_SIM_FIND_NEXT_EVENT(sim, out_event_data, player_collided_level, sent_player_collided_level);
}
