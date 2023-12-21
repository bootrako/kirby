#include <btk_sim.h>
#include "btk_input.h"
#include "btk_level.h"
#include "btk_player.h"

struct btk_sim_t {
    btk_ctx ctx;
    btk_input input;
    btk_level level;
    btk_player player;
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

    btk_level_load(&sim->ctx, &sim->level, BTK_DATA_GREEN_GREENS, &sim->player);

    return sim;
}

void btk_sim_deinit(btk_sim* sim) {
    btk_level_deinit(&sim->ctx, &sim->level);
    btk_ctx_free(&sim->ctx, sim);
}

static void btk_sim_update_frame(btk_sim* sim) {
    btk_ctx_update(&sim->ctx);
    btk_input_update(&sim->ctx, &sim->input);
    btk_player_update(&sim->ctx, &sim->player); 
}

void btk_sim_update(btk_sim* sim, float delta_time) {
    float time_per_frame = 1.0f / 60.0f;
    int max_frames_per_update = 10;
    float max_delta_time = time_per_frame * max_frames_per_update;

    sim->frame_accumulator += BTK_MIN(delta_time, max_delta_time);
    while (sim->frame_accumulator > time_per_frame) {
        btk_sim_update_frame(sim);
        sim->frame_accumulator -= time_per_frame;
    }
}

void btk_sim_get_player_pos(btk_sim* sim, int* out_x, int* out_y) {
    *out_x = (int)sim->player.xform.x;
    *out_y = (int)sim->player.xform.y;
}