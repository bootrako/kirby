#include "godot_sim.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

KirbySim::KirbySim() {
}

KirbySim::~KirbySim() {
}

void KirbySim::_bind_methods() {
	ClassDB::bind_method(D_METHOD("sim_init"), &KirbySim::_sim_init);
	ClassDB::bind_method(D_METHOD("sim_deinit"), &KirbySim::_sim_deinit);
}

void KirbySim::_sim_init() {
	kirby_sim_host host;
	host.alloc = KirbySim::_godot_alloc;
	host.free = KirbySim::_godot_free;
	sim = kirby_sim_init(host);
}

void KirbySim::_sim_deinit() {
	kirby_sim_deinit(sim);
}

void* KirbySim::_godot_alloc(void* context, int size) {
    return memalloc(size);
}

void KirbySim::_godot_free(void* context, void* ptr) {
    memfree(ptr);
}