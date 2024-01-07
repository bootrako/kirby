#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/node.hpp>

namespace godot {
    class Player : public Node {
        GDCLASS(Player, Node)

    public:
        void _process(double delta);

    protected:
        static void _bind_methods();
    
    private:
        NodePath get_sim_path() const;
        void set_sim_path(const NodePath& sim_path);

        NodePath get_sprite_path() const;
        void set_sprite_path(const NodePath& sprite_path);

        NodePath get_anim_tree_path() const;
        void set_anim_tree_path(const NodePath& anim_tree_path);

        NodePath get_small_star_path() const;
        void set_small_star_path(const NodePath& small_star_path);

        float get_run_anim_vel() const;
        void set_run_anim_vel(float run_anim_vel);

        float get_splat_h_vel() const;
        void set_splat_h_vel(float splat_h_vel);

        bool get_is_running() const;
        void set_is_running(bool is_running);

        bool get_is_falling() const;
        void set_is_falling(bool is_falling);

        bool get_is_diving() const;
        void set_is_diving(bool is_diving);

        bool get_is_dive_stunned() const;
        void set_is_dive_stunned(bool is_dive_stunned);

        bool get_is_crouching() const;
        void set_is_crouching(bool is_crouching);

        bool get_is_splat_h() const;
        void set_is_splat_h(bool is_splat_h);

        bool get_is_splat_v() const;
        void set_is_splat_v(bool is_splat_v);

        NodePath sim_path;
        NodePath sprite_path;
        NodePath anim_tree_path;
        NodePath small_star_path;
        float run_anim_vel = false;
        float splat_h_vel = false;
        bool is_running = false;
        bool is_falling = false;
        bool is_diving = false;
        bool is_crouching = false;
        bool is_splat_h = false;
        bool is_splat_v = false;
        bool is_dive_stunned = false;
    };   
}

#endif // PLAYER_H