#include "godot_sim.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

KirbySim::KirbySim() {
}

KirbySim::~KirbySim() {
}

void KirbySim::_bind_methods() {
	ClassDB::bind_method(D_METHOD("sim_init"), &KirbySim::_sim_init);
	ClassDB::bind_method(D_METHOD("sim_deinit"), &KirbySim::_sim_deinit);
	ClassDB::bind_method(D_METHOD("sim_update"), &KirbySim::_sim_update);
	ClassDB::bind_method(D_METHOD("sim_get_player_pos"), &KirbySim::_sim_get_player_pos);
}

void KirbySim::_sim_init() {
	kirby_sim_host host;
	host.alloc = KirbySim::_godot_alloc;
	host.free = KirbySim::_godot_free;
	host.panic = KirbySim::_godot_panic;
	host.input_action_pressed = KirbySim::_godot_input_action_pressed;
	sim = kirby_sim_init(host);
}

void KirbySim::_sim_deinit() {
	kirby_sim_deinit(sim);
}

int KirbySim::_sim_update(float delta_time) {
	return kirby_sim_update(sim, delta_time);
}

Vector2i KirbySim::_sim_get_player_pos() const {
	kirby_sim_pos pos = kirby_sim_get_player_pos(sim);
	return Vector2i(pos.x, pos.y);
}

void* KirbySim::_godot_alloc(void* context, int size) {
    return memalloc(size);
}

void KirbySim::_godot_free(void* context, void* ptr) {
    memfree(ptr);
}

void KirbySim::_godot_panic(void* context, const char* msg) {
    CRASH_NOW_MSG(msg);
}

bool KirbySim::_godot_input_action_pressed(void* context, const kirby_sim_input_action input_action) {
	const char* input_action_to_string[] = {
		"move_left",	// KIRBY_SIM_INPUT_ACTION_MOVE_LEFT
		"move_right",	// KIRBY_SIM_INPUT_ACTION_MOVE_RIGHT
		"move_up",		// KIRBY_SIM_INPUT_ACTION_MOVE_UP
		"move_down",	// KIRBY_SIM_INPUT_ACTION_MOVE_DOWN
	};
	return Input::get_singleton()->is_action_pressed(input_action_to_string[input_action]);
}