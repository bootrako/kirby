#include "godot_sim.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

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
    host.read_cfg = GodotSim::_godot_read_cfg;
    host.ctx = this;
    sim = btk_sim_init(host);
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

    set_info();
}

Dictionary GodotSim::get_info() const {
    return info;
}

void GodotSim::set_info() {
    btk_sim_vec player_pos = btk_sim_get_player_pos(sim);
    info["player_pos"] = Vector2(player_pos.x, player_pos.y);
}

void GodotSim::set_cfg(Dictionary cfg) {
    this->cfg = cfg;
}

Dictionary GodotSim::get_cfg() const {
    return cfg;
}

void GodotSim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_info"), &GodotSim::get_info);

    ClassDB::bind_method(D_METHOD("set_cfg", "cfg"), &GodotSim::set_cfg);
    ClassDB::bind_method(D_METHOD("get_cfg"), &GodotSim::get_cfg);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "cfg"), "set_cfg", "get_cfg");
}

void* GodotSim::_godot_alloc(void* ctx, int size) {
    return memalloc(size);
}

void GodotSim::_godot_free(void* ctx, void* ptr) {
    memfree(ptr);
}

void GodotSim::_godot_panic(void* ctx, char* err_msg) {
    CRASH_NOW_MSG(err_msg);
}

void GodotSim::_godot_log(void* ctx, char* msg) {
    UtilityFunctions::print(msg);
}

bool GodotSim::_godot_is_action_active(void* ctx, btk_action action) {
    const char* input_action_to_string[] = {
        "move_left",    // BTK_ACTION_MOVE_LEFT
        "move_right",   // BTK_ACTION_MOVE_RIGHT
        "float",        // BTK_ACTION_FLOAT
        "crouch",       // BTK_ACTION_CROUCH
        "jump",         // BTK_ACTION_JUMP
        "inhale",       // BTK_ACTION_INHALE
    };
    return Input::get_singleton()->is_action_pressed(input_action_to_string[action]);
}

char* GodotSim::_godot_read_data(void* ctx, btk_data data, int* out_len) {
    const char* data_to_path[] = {
        "res://data/green_greens.txt"   // BTK_DATA_GREEN_GREENS
    };
    const PackedByteArray pba = FileAccess::get_file_as_bytes(data_to_path[data]);
    *out_len = static_cast<int>(pba.size());
    return reinterpret_cast<char*>(const_cast<uint8_t*>(pba.ptr()));
}

void GodotSim::_godot_read_cfg(void* ctx, btk_cfg* cfg) {
    GodotSim* godot_sim = static_cast<GodotSim*>(ctx);

    Vector2 accel = godot_sim->cfg["accel"];
    cfg->player_accel.x = accel.x;
    cfg->player_accel.y = accel.y;

    Vector2 vel_max = godot_sim->cfg["vel max"];
    cfg->player_vel_max.x = vel_max.x;
    cfg->player_vel_max.y = vel_max.y;

    cfg->player_vel_damp_x = godot_sim->cfg["vel damp x"];
    cfg->player_gravity = godot_sim->cfg["gravity"];
    cfg->player_max_jump_timer = godot_sim->cfg["max jump timer"];
}