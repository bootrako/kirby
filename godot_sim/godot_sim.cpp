#include "godot_sim.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <stdlib.h>

using namespace godot;

GodotSim::GodotSim() : sim(nullptr) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    btk_host host;
    host.alloc = GodotSim::_godot_alloc;
    host.free = GodotSim::_godot_free;
    host.panic = GodotSim::_godot_panic;
    host.log = GodotSim::_godot_log;
    host.is_action_active = GodotSim::_godot_is_action_active;
    host.read_data = GodotSim::_godot_read_data;
    sim = btk_sim_init(&host);
}

GodotSim::~GodotSim() {
    if (sim != nullptr) {
        btk_sim_deinit(sim);
    }
}

void GodotSim::_process(double delta_time) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (sim != nullptr) {
        btk_sim_update(sim, delta_time);        
    }
}

void GodotSim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_player_pos"), &GodotSim::_sim_get_player_pos);
}

Vector2i GodotSim::_sim_get_player_pos() const {
    int x, y;
    btk_sim_get_player_pos(sim, &x, &y);
    return Vector2i(x, y);
}

void* GodotSim::_godot_alloc(void* context, int size) {
    return memalloc(size);
}

void GodotSim::_godot_free(void* context, void* ptr) {
    memfree(ptr);
}

void GodotSim::_godot_panic(void* context, const char* err_msg) {
    CRASH_NOW_MSG(err_msg);
}

void GodotSim::_godot_log(void* context, const char* msg) {
    UtilityFunctions::print(msg);
}

bool GodotSim::_godot_is_action_active(void* context, btk_action action) {
    const char* input_action_to_string[] = {
        "move_left",    // BTK_ACTION_MOVE_LEFT
        "move_right",   // BTK_ACTION_MOVE_RIGHT
        "move_up",      // BTK_ACTION_MOVE_UP
        "move_down",    // BTK_ACTION_MOVE_DOWN
    };
    return Input::get_singleton()->is_action_pressed(input_action_to_string[action]);
}

const char* GodotSim::_godot_read_data(void* context, btk_data data, int* out_len) {
    const char* data_to_path[] = {
        "res://data/green_greens.txt"   // BTK_DATA_GREEN_GREENS
    };
    const PackedByteArray pba = FileAccess::get_file_as_bytes(data_to_path[data]);
    if (out_len) {
        *out_len = static_cast<int>(pba.size());
    }
    return reinterpret_cast<const char*>(pba.ptr());
}