#include "player.h"
#include "sim.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void Player::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    is_running = false;
    is_falling = false;
}

void Player::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    btk_sim* sim = get_node<Sim>(sim_node_path)->get_sim();

    btk_vec pos = btk_sim_get_player_pos(sim);
    set_position(Vector2(pos.x, pos.y));

    Input* input = Input::get_singleton();
    if (input->is_action_pressed("move_left") && !input->is_action_pressed("move_right")) {
        set_flip_h(true);
    }
    if (input->is_action_pressed("move_right") && !input->is_action_pressed("move_left")) {
        set_flip_h(false);
    }

    is_running = false;
    is_falling = false;
    bool is_grounded = btk_sim_get_player_is_grounded(sim);
    if (is_grounded) {
        btk_vec vel = btk_sim_get_player_vel(sim);
        is_running = Math::absf(vel.x) > run_anim_vel;
    } else {
        is_falling = true;
    }
}

void Player::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sim_node_path"), &Player::get_sim_node_path);
    ClassDB::bind_method(D_METHOD("set_sim_node_path", "sim_node_path"), &Player::set_sim_node_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "sim", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Sim"), "set_sim_node_path", "get_sim_node_path");

    ClassDB::bind_method(D_METHOD("get_run_anim_vel"), &Player::get_run_anim_vel);
    ClassDB::bind_method(D_METHOD("set_run_anim_vel", "run_anim_vel"), &Player::set_run_anim_vel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "run_anim_vel"), "set_run_anim_vel", "get_run_anim_vel");

    ClassDB::bind_method(D_METHOD("get_is_running"), &Player::get_is_running);
    ClassDB::bind_method(D_METHOD("set_is_running", "is_running"), &Player::set_is_running);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_running", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_is_running", "get_is_running");

    ClassDB::bind_method(D_METHOD("get_is_falling"), &Player::get_is_falling);
    ClassDB::bind_method(D_METHOD("set_is_falling", "is_falling"), &Player::set_is_falling);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_falling", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_is_falling", "get_is_falling");
}

NodePath Player::get_sim_node_path() const {
    return sim_node_path;
}

void Player::set_sim_node_path(const NodePath& sim_node_path) {
    this->sim_node_path = sim_node_path;
}

float Player::get_run_anim_vel() const {
    return run_anim_vel;
}

void Player::set_run_anim_vel(float run_anim_vel) {
    this->run_anim_vel = run_anim_vel;
}

bool Player::get_is_running() const {
    return is_running;
}

void Player::set_is_running(bool is_running) {
    this->is_running = is_running;
}

bool Player::get_is_falling() const {
    return is_falling;
}

void Player::set_is_falling(bool is_falling) {
    this->is_falling = is_falling;
}