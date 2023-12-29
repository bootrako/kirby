#include <btk_sim.h>
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
    if (sim == NULL) {
        host.panic(host.ctx, "failed to alloc sim!");
    }

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
    sim->ctx.host.free(sim->ctx.host.ctx, sim);
}

void btk_sim_update(btk_sim* sim, float delta_time) {
    int update_frame = 0;

    sim->frame_accumulator = btk_minf(sim->frame_accumulator + delta_time, BTK_SIM_MAX_FRAMES_PER_UPDATE * BTK_DT);
    while (sim->frame_accumulator > BTK_DT && update_frame < BTK_SIM_MAX_FRAMES_PER_UPDATE) {
        btk_ctx_update(&sim->ctx);
        btk_input_update(&sim->ctx, &sim->input);
        btk_player_update(&sim->ctx, &sim->player); 

        sim->events_since_last_update[update_frame] = sim->ctx.events;
        sim->frame_accumulator -= BTK_DT;

        update_frame++;
    }

    sim->frames_since_last_update = update_frame;
}

btk_sim_vec btk_sim_get_player_pos(btk_sim* sim) {
    return (btk_sim_vec){ .x = sim->player.xform.x, .y = sim->player.xform.y };
}

btk_sim_vec btk_sim_get_player_vel(btk_sim* sim) {
    return (btk_sim_vec){ .x = sim->player.vel.x, .y = sim->player.vel.y };
}

bool btk_sim_get_player_is_grounded(btk_sim* sim) {
    return sim->player.is_grounded;
}

int btk_sim_get_event_player_landed(btk_sim* sim) {
    int player_landed_count = 0;
    for (int i = 0; i < sim->frames_since_last_update; ++i) {
        player_landed_count += sim->events_since_last_update[i].player_landed_count;
    }
    return player_landed_count;
}