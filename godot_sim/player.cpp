#include "player.h"
#include "sim.h"
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/cpu_particles2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

using namespace godot;

void Player::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    btk_sim* sim = get_node<Sim>(sim_path)->get_sim();
    btk_cfg* cfg = btk_sim_get_cfg(sim);
    Sprite2D* sprite = get_node<Sprite2D>(sprite_path);

    btk_vec pos = btk_sim_get_player_pos(sim);
    sprite->set_position(Vector2(pos.x, pos.y));

    is_crouching = btk_sim_get_player_is_crouching(sim);
    
    Input* input = Input::get_singleton();
    if (input->is_action_pressed("move_left") && !input->is_action_pressed("move_right") && !is_crouching) {
        sprite->set_flip_h(true);
    }
    if (input->is_action_pressed("move_right") && !input->is_action_pressed("move_left") && !is_crouching) {
        sprite->set_flip_h(false);
    }

    bool was_falling = is_falling;
    is_running = false;
    is_falling = false;
    if (btk_sim_get_player_is_grounded(sim)) {
        btk_vec vel = btk_sim_get_player_vel(sim);
        is_running = Math::absf(vel.x) > run_anim_vel;
    } else {
        is_falling = true;
    }

    is_diving = btk_sim_get_player_fall_timer(sim) > cfg->player_fall_dive_timer;

    is_splat_v = false;
    is_splat_h = false;
    btk_event_player_collided_level* player_collided_level = nullptr;
    while (btk_sim_get_event_player_collided_level(sim, &player_collided_level)) {
        is_splat_h |= player_collided_level->normal.x != 0.0f && Math::absf(player_collided_level->vel.x) > splat_h_vel;
        is_splat_v |= player_collided_level->normal.y == 1.0f;
    }
    is_splat_v |= was_falling && !is_falling;

    bool was_dive_stunned = is_dive_stunned;
    is_dive_stunned = btk_sim_get_player_is_dive_stunned(sim);

    if ((is_splat_h || is_splat_v) && !was_dive_stunned) {
        CPUParticles2D* small_star = get_node<CPUParticles2D>(small_star_path);
        small_star->set_emitting(true);
    }

    float offset_x = 0.0f;
    float offset_y = 0.0f;
    AnimationTree* anim_tree = get_node<AnimationTree>(anim_tree_path);
    AnimationNodeStateMachinePlayback* state_machine_playback = Object::cast_to<AnimationNodeStateMachinePlayback>(anim_tree->get("parameters/small/playback"));
    if (state_machine_playback->is_playing() && state_machine_playback->get_current_node() == StringName("splat_h")) {
        offset_x = sprite->is_flipped_h() ? -4.0f : 4.0f;
    }
    if (state_machine_playback->is_playing() && state_machine_playback->get_current_node() == StringName("splat_v") && is_falling) {
        offset_y = -8.0f;
    }
    if (is_crouching) {
        offset_y = -8.0f;
    }
    // Ref<Texture2D> tex = sprite->get_texture();
    // if (tex.is_valid()) {
    //     offset_x += (16.0f - tex->get_width()) / 2.0f;
    //     offset_y += 16.0f - tex->get_height();
    // }
    sprite->set_offset(Vector2(offset_x, offset_y));
}

void Player::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sim_path"), &Player::get_sim_path);
    ClassDB::bind_method(D_METHOD("set_sim_path", "sim_path"), &Player::set_sim_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "sim", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Sim"), "set_sim_path", "get_sim_path");

    ClassDB::bind_method(D_METHOD("get_sprite_path"), &Player::get_sprite_path);
    ClassDB::bind_method(D_METHOD("set_sprite_path", "sprite_path"), &Player::set_sprite_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "sprite", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Sprite2D"), "set_sprite_path", "get_sprite_path");

    ClassDB::bind_method(D_METHOD("get_anim_tree_path"), &Player::get_anim_tree_path);
    ClassDB::bind_method(D_METHOD("set_anim_tree_path", "anim_tree_path"), &Player::set_anim_tree_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "anim_tree", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AnimationTree"), "set_anim_tree_path", "get_anim_tree_path");

    ClassDB::bind_method(D_METHOD("get_small_star_path"), &Player::get_small_star_path);
    ClassDB::bind_method(D_METHOD("set_small_star_path", "small_star_path"), &Player::set_small_star_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "small_star", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CPUParticles2D"), "set_small_star_path", "get_small_star_path");

    ClassDB::bind_method(D_METHOD("get_run_anim_vel"), &Player::get_run_anim_vel);
    ClassDB::bind_method(D_METHOD("set_run_anim_vel", "run_anim_vel"), &Player::set_run_anim_vel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "run_anim_vel"), "set_run_anim_vel", "get_run_anim_vel");

    ClassDB::bind_method(D_METHOD("get_splat_h_vel"), &Player::get_splat_h_vel);
    ClassDB::bind_method(D_METHOD("set_splat_h_vel", "splat_h_vel"), &Player::set_splat_h_vel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "splat_h_vel"), "set_splat_h_vel", "get_splat_h_vel");

    ClassDB::bind_method(D_METHOD("get_is_running"), &Player::get_is_running);
    ClassDB::bind_method(D_METHOD("set_is_running", "is_running"), &Player::set_is_running);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_running", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_running", "get_is_running");

    ClassDB::bind_method(D_METHOD("get_is_falling"), &Player::get_is_falling);
    ClassDB::bind_method(D_METHOD("set_is_falling", "is_falling"), &Player::set_is_falling);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_falling", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_falling", "get_is_falling");

    ClassDB::bind_method(D_METHOD("get_is_diving"), &Player::get_is_diving);
    ClassDB::bind_method(D_METHOD("set_is_diving", "is_diving"), &Player::set_is_diving);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_diving", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_diving", "get_is_diving");

    ClassDB::bind_method(D_METHOD("get_is_dive_stunned"), &Player::get_is_dive_stunned);
    ClassDB::bind_method(D_METHOD("set_is_dive_stunned", "is_dive_stunned"), &Player::set_is_dive_stunned);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_dive_stunned", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_dive_stunned", "get_is_dive_stunned");

    ClassDB::bind_method(D_METHOD("get_is_crouching"), &Player::get_is_crouching);
    ClassDB::bind_method(D_METHOD("set_is_crouching", "is_crouching"), &Player::set_is_crouching);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_crouching", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_crouching", "get_is_crouching");

    ClassDB::bind_method(D_METHOD("get_is_splat_h"), &Player::get_is_splat_h);
    ClassDB::bind_method(D_METHOD("set_is_splat_h", "is_splat_h"), &Player::set_is_splat_h);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_splat_h", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_splat_h", "get_is_splat_h");

    ClassDB::bind_method(D_METHOD("get_is_splat_v"), &Player::get_is_splat_v);
    ClassDB::bind_method(D_METHOD("set_is_splat_v", "is_splat_v"), &Player::set_is_splat_v);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_splat_v", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_is_splat_v", "get_is_splat_v");
}

NodePath Player::get_sim_path() const {
    return sim_path;
}

void Player::set_sim_path(const NodePath& sim_path) {
    this->sim_path = sim_path;
}

NodePath Player::get_sprite_path() const {
    return sprite_path;
}

void Player::set_sprite_path(const NodePath& sprite_path) {
    this->sprite_path = sprite_path;
}

NodePath Player::get_anim_tree_path() const {
    return anim_tree_path;
}

void Player::set_anim_tree_path(const NodePath& anim_tree_path) {
    this->anim_tree_path = anim_tree_path;
}

NodePath Player::get_small_star_path() const {
    return small_star_path;
}

void Player::set_small_star_path(const NodePath& small_star_path) {
    this->small_star_path = small_star_path;
}

float Player::get_run_anim_vel() const {
    return run_anim_vel;
}

void Player::set_run_anim_vel(float run_anim_vel) {
    this->run_anim_vel = run_anim_vel;
}

float Player::get_splat_h_vel() const {
    return splat_h_vel;
}

void Player::set_splat_h_vel(float splat_h_vel) {
    this->splat_h_vel = splat_h_vel;
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

bool Player::get_is_diving() const {
    return is_diving;
}

void Player::set_is_diving(bool is_diving) {
    this->is_diving = is_diving;
}

bool Player::get_is_dive_stunned() const {
    return is_dive_stunned;
}

void Player::set_is_dive_stunned(bool is_dive_stunned) {
    this->is_dive_stunned = is_dive_stunned;
}

bool Player::get_is_crouching() const {
    return is_crouching;
}

void Player::set_is_crouching(bool is_crouching) {
    this->is_crouching = is_crouching;
}

bool Player::get_is_splat_h() const {
    return is_splat_h;
}

void Player::set_is_splat_h(bool is_splat_h) {
    this->is_splat_h = is_splat_h;
}

bool Player::get_is_splat_v() const {
    return is_splat_v;
}

void Player::set_is_splat_v(bool is_splat_h) {
    this->is_splat_v = is_splat_v;
}