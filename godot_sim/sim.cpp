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

void Sim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_player_accel"), &Sim::get_player_accel);
    ClassDB::bind_method(D_METHOD("set_player_accel", "player_accel"), &Sim::set_player_accel);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_accel"), "set_player_accel", "get_player_accel");

    ClassDB::bind_method(D_METHOD("get_player_vel_min"), &Sim::get_player_vel_min);
    ClassDB::bind_method(D_METHOD("set_player_vel_min", "player_vel_min"), &Sim::set_player_vel_min);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_vel_min"), "set_player_vel_min", "get_player_vel_min");

    ClassDB::bind_method(D_METHOD("get_player_vel_max"), &Sim::get_player_vel_max);
    ClassDB::bind_method(D_METHOD("set_player_vel_max", "player_vel_max"), &Sim::set_player_vel_max);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "player_vel_max"), "set_player_vel_max", "get_player_vel_max");

    ClassDB::bind_method(D_METHOD("get_player_vel_damp_x"), &Sim::get_player_vel_damp_x);
    ClassDB::bind_method(D_METHOD("set_player_vel_damp_x", "player_vel_damp_x"), &Sim::set_player_vel_damp_x);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_vel_damp_x"), "set_player_vel_damp_x", "get_player_vel_damp_x");

    ClassDB::bind_method(D_METHOD("get_player_jump_release_vel_y"), &Sim::get_player_jump_release_vel_y);
    ClassDB::bind_method(D_METHOD("set_player_jump_release_vel_y", "player_jump_release_vel_y"), &Sim::set_player_jump_release_vel_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_jump_release_vel_y"), "set_player_jump_release_vel_y", "get_player_jump_release_vel_y");

    ClassDB::bind_method(D_METHOD("get_player_gravity"), &Sim::get_player_gravity);
    ClassDB::bind_method(D_METHOD("set_player_gravity", "player_gravity"), &Sim::set_player_gravity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_gravity"), "set_player_gravity", "get_player_gravity");

    ClassDB::bind_method(D_METHOD("get_player_gravity_dive_stunned"), &Sim::get_player_gravity_dive_stunned);
    ClassDB::bind_method(D_METHOD("set_player_gravity_dive_stunned", "player_gravity_dive_stunned"), &Sim::set_player_gravity_dive_stunned);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_gravity_dive_stunned"), "set_player_gravity_dive_stunned", "get_player_gravity_dive_stunned");

    ClassDB::bind_method(D_METHOD("get_player_max_jump_timer"), &Sim::get_player_max_jump_timer);
    ClassDB::bind_method(D_METHOD("set_player_max_jump_timer", "player_max_jump_timer"), &Sim::set_player_max_jump_timer);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_max_jump_timer"), "set_player_max_jump_timer", "get_player_max_jump_timer");

    ClassDB::bind_method(D_METHOD("get_player_fall_dive_timer"), &Sim::get_player_fall_dive_timer);
    ClassDB::bind_method(D_METHOD("set_player_fall_dive_timer", "player_fall_dive_timer"), &Sim::set_player_fall_dive_timer);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_fall_dive_timer"), "set_player_fall_dive_timer", "get_player_fall_dive_timer");

    ClassDB::bind_method(D_METHOD("get_player_dive_bounce_vel_y"), &Sim::get_player_dive_bounce_vel_y);
    ClassDB::bind_method(D_METHOD("set_player_dive_bounce_vel_y", "player_dive_bounce_vel_y"), &Sim::set_player_dive_bounce_vel_y);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_dive_bounce_vel_y"), "set_player_dive_bounce_vel_y", "get_player_dive_bounce_vel_y");
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
    *cfg = godot_sim->cfg;
}


Vector2 Sim::get_player_accel() const {
    return Vector2(cfg.player_accel.x, cfg.player_accel.y);
}

void Sim::set_player_accel(const Vector2& player_accel) {
    cfg.player_accel.x = player_accel.x;
    cfg.player_accel.y = player_accel.y;
}

Vector2 Sim::get_player_vel_min() const {
    return Vector2(cfg.player_vel_min.x, cfg.player_vel_min.y);
}

void Sim::set_player_vel_min(const Vector2& player_vel_min) {
    cfg.player_vel_min.x = player_vel_min.x;
    cfg.player_vel_min.y = player_vel_min.y;
}

Vector2 Sim::get_player_vel_max() const {
    return Vector2(cfg.player_vel_max.x, cfg.player_vel_max.y);
}

void Sim::set_player_vel_max(const Vector2& player_vel_max) {
    cfg.player_vel_max.x = player_vel_max.x;
    cfg.player_vel_max.y = player_vel_max.y;
}

float Sim::get_player_vel_damp_x() const {
    return cfg.player_vel_damp_x;
}

void Sim::set_player_vel_damp_x(float player_vel_damp_x) {
    cfg.player_vel_damp_x = player_vel_damp_x;
}

float Sim::get_player_jump_release_vel_y() const {
    return cfg.player_jump_release_vel_y;
}

void Sim::set_player_jump_release_vel_y(float player_jump_release_vel_y) {
    cfg.player_jump_release_vel_y = player_jump_release_vel_y;
}

float Sim::get_player_gravity() const {
    return cfg.player_gravity;
}

void Sim::set_player_gravity(float player_gravity) {
    cfg.player_gravity = player_gravity;
}

float Sim::get_player_gravity_dive_stunned() const {
    return cfg.player_gravity_dive_stunned;
}

void Sim::set_player_gravity_dive_stunned(float player_gravity_dive_stunned) {
    cfg.player_gravity_dive_stunned = player_gravity_dive_stunned;
}

float Sim::get_player_max_jump_timer() const {
    return cfg.player_max_jump_timer;
}

void Sim::set_player_max_jump_timer(float player_max_jump_timer) {
    cfg.player_max_jump_timer = player_max_jump_timer;
}

float Sim::get_player_fall_dive_timer() const {
    return cfg.player_fall_dive_timer;
}

void Sim::set_player_fall_dive_timer(float player_fall_dive_timer) {
    cfg.player_fall_dive_timer = player_fall_dive_timer;
}

float Sim::get_player_dive_bounce_vel_y() const {
    return cfg.player_dive_bounce_vel_y;
}

void Sim::set_player_dive_bounce_vel_y(float player_dive_bounce_vel_y) {
    cfg.player_dive_bounce_vel_y = player_dive_bounce_vel_y;
}