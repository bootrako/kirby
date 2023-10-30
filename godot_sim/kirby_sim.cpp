#include "kirby_sim.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

KirbySim::KirbySim() {
}

KirbySim::~KirbySim() {
}

void KirbySim::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_number"), &KirbySim::_get_number);
}

int KirbySim::_get_number() {
	return 41;
}
