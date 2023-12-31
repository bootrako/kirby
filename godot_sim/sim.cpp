#include "sim.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Sim::Sim() : sim(nullptr) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    btk_host host;
    host.alloc = Sim::_alloc;
    host.free = Sim::_free;
    host.panic = Sim::_panic;
    host.log = Sim::_log;
    host.is_action_pressed = Sim::_is_action_pressed;
    host.read_data = Sim::_read_data;
    host.read_cfg = Sim::_read_cfg;
    host.ctx = this;
    sim = btk_sim_init(host);
}

Sim::~Sim() {
    if (sim != nullptr) {
        btk_sim_deinit(sim);
    }
}

void Sim::_process(double delta_time) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    if (sim != nullptr) {
        btk_sim_update(sim, delta_time);        
    }
}

btk_sim* Sim::get_sim() const {
     return sim;
}

Dictionary Sim::get_cfg() const {
    return cfg;
}

void Sim::set_cfg(Dictionary cfg) {
    this->cfg = cfg;
}

void Sim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_cfg"), &Sim::get_cfg);
    ClassDB::bind_method(D_METHOD("set_cfg", "cfg"), &Sim::set_cfg);
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "cfg"), "set_cfg", "get_cfg");
}

void* Sim::_alloc(void* ctx, int size) {
    return memalloc(size);
}

void Sim::_free(void* ctx, void* ptr) {
    memfree(ptr);
}

void Sim::_panic(void* ctx, char* err_msg) {
    CRASH_NOW_MSG(err_msg);
}

void Sim::_log(void* ctx, char* msg) {
    UtilityFunctions::print(msg);
}

bool Sim::_is_action_pressed(void* ctx, btk_action action) {
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

char* Sim::_read_data(void* ctx, btk_data data, int* out_len) {
    const char* data_to_path[] = {
        "res://data/green_greens.txt"   // BTK_DATA_GREEN_GREENS
    };
    const PackedByteArray pba = FileAccess::get_file_as_bytes(data_to_path[data]);
    *out_len = static_cast<int>(pba.size());
    return reinterpret_cast<char*>(const_cast<uint8_t*>(pba.ptr()));
}

void Sim::_read_cfg(void* ctx, btk_cfg* cfg) {
    Sim* godot_sim = static_cast<Sim*>(ctx);

    Vector2 accel = godot_sim->cfg["player_accel"];
    cfg->player_accel.x = accel.x;
    cfg->player_accel.y = accel.y;

    Vector2 vel_min = godot_sim->cfg["player_vel_min"];
    cfg->player_vel_min.x = vel_min.x;
    cfg->player_vel_min.y = vel_min.y;

    Vector2 vel_max = godot_sim->cfg["player_vel_max"];
    cfg->player_vel_max.x = vel_max.x;
    cfg->player_vel_max.y = vel_max.y;

    cfg->player_vel_damp_x = godot_sim->cfg["player_vel_damp_x"];
    cfg->player_jump_release_vel_y = godot_sim->cfg["player_jump_release_vel_y"];
    cfg->player_gravity = godot_sim->cfg["player_gravity"];
    cfg->player_max_jump_timer = godot_sim->cfg["player_max_jump_timer"];
}