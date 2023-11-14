#include <btk_sim.h>
#include "btk_ctx.h"
#include "btk_input.h"
#include "btk_level.h"
#include "btk_player.h"
#include "btk_utils.h"

struct btk_sim_t {
    btk_input input;
    btk_level level;
    btk_player player;
    float frame_accumulator;
};

btk_sim* btk_sim_init(const btk_host* host) {
    btk_ctx_init(host);

    btk_sim* sim = host->alloc(host->ctx, sizeof(btk_sim));
    if (sim == NULL) {
        host->panic(host->ctx, "failed to alloc sim!");
    }
    btk_input_init(&sim->input);
    btk_level_init(&sim->level);
    btk_player_init(&sim->player, &sim->input);
    sim->frame_accumulator = 0.0f;

    btk_level_load(&sim->level, BTK_DATA_GREEN_GREENS, &sim->player);

    return sim;
}

void btk_sim_deinit(btk_sim* sim) {
    btk_level_deinit(&sim->level);

    const btk_host* host = btk_ctx_host();
    host->free(host->ctx, sim);
}

static void btk_sim_update_frame(btk_sim* sim) {
    btk_input_update(&sim->input);
    btk_player_update(&sim->player); 
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
        *out_x = sim->player.xform.x;
    }

    if (out_y) {
        *out_y = sim->player.xform.y;
    }
}