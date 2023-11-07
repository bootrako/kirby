#include <btk_sim.h>
#include "btk_input.h"
#include "btk_player.h"
#include "btk_utils.h"

struct btk_sim {
    btk_sim_host host;
    btk_input input;
    btk_player player;
    float frame_accumulator;
};

btk_sim* btk_sim_init(btk_sim_host host) {
    btk_sim* sim = host.alloc(host.context, sizeof(btk_sim));
    sim->host = host;
    
    btk_input_init(&sim->input, &sim->host);
    btk_player_init(&sim->player);
    sim->frame_accumulator = 0.0f;

    return sim;
}

void btk_sim_deinit(btk_sim* sim) {
    const btk_sim_host host = sim->host;
    host.free(host.context, sim);
}

static void btk_sim_update_frame(btk_sim* sim) {
    btk_input_update(&sim->input);
    btk_player_update(&sim->player, &sim->input); 
}

void btk_sim_update(btk_sim* sim, float delta_time) {
    const float time_per_frame = 1.0f / 60.0f;
    const int max_frames_per_update = 10;
    const float max_delta_time = time_per_frame * max_frames_per_update;

    sim->frame_accumulator += BTK_MIN(delta_time, max_delta_time);
    while (sim->frame_accumulator > time_per_frame) {
        btk_sim_update_frame(sim);
        sim->frame_accumulator -= time_per_frame;
    }
}

void btk_sim_get_player_pos(const btk_sim* sim, int* out_x, int* out_y) {
    if (out_x) {
        *out_x = sim->player.pos.x;
    }

    if (out_y) {
        *out_y = sim->player.pos.y;
    }
}