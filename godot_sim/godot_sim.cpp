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
}

void GodotSim::set_player_accel(Vector2 player_accel) {
    cfg.player_accel[0] = player_accel.x;
    cfg.player_accel[1] = player_accel.y;
}

Vector2 GodotSim::get_player_accel() const {
    return Vector2(cfg.player_accel[0], cfg.player_accel[1]);
}

void GodotSim::set_player_vel_max(Vector2 player_vel_max) {
    cfg.player_vel_max[0] = player_vel_max.x;
    cfg.player_vel_max[1] = player_vel_max.y;
}

Vector2 GodotSim::get_player_vel_max() const {
    return Vector2(cfg.player_vel_max[0], cfg.player_vel_max[1]);
}

void GodotSim::set_player_vel_damp(Vector2 player_vel_damp) {
    cfg.player_vel_damp[0] = player_vel_damp.x;
    cfg.player_vel_damp[1] = player_vel_damp.y;
}

Vector2 GodotSim::get_player_vel_damp() const {
    return Vector2(cfg.player_vel_damp[0], cfg.player_vel_damp[1]);
}

void GodotSim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_player_pos"), &GodotSim::_sim_get_player_pos);

    ClassDB::bind_method(D_METHOD("set_player_accel", "player_accel"), &GodotSim::set_player_accel);
    ClassDB::bind_method(D_METHOD("get_player_accel"), &GodotSim::get_player_accel);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_accel"), "set_player_accel", "get_player_accel");

    ClassDB::bind_method(D_METHOD("set_player_vel_max", "player_vel_max"), &GodotSim::set_player_vel_max);
    ClassDB::bind_method(D_METHOD("get_player_vel_max"), &GodotSim::get_player_vel_max);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_vel_max"), "set_player_vel_max", "get_player_vel_max");

    ClassDB::bind_method(D_METHOD("set_player_vel_damp", "player_vel_damp"), &GodotSim::set_player_vel_damp);
    ClassDB::bind_method(D_METHOD("get_player_vel_damp"), &GodotSim::get_player_vel_damp);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_vel_damp"), "set_player_vel_damp", "get_player_vel_damp");
}

Vector2i GodotSim::_sim_get_player_pos() const {
    int x, y;
    btk_sim_get_player_pos(sim, &x, &y);
    return Vector2i(x, y);
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
        "move_up",      // BTK_ACTION_MOVE_UP
        "move_down",    // BTK_ACTION_MOVE_DOWN
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
    *cfg = godot_sim->cfg;
}