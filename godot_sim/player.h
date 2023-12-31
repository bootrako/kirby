#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot {
    class Player : public Sprite2D {
        GDCLASS(Player, Sprite2D)

    public:
        void _ready();
        void _process(double delta);

    protected:
        static void _bind_methods();
    
    private:
        NodePath get_sim_node_path() const;
        void set_sim_node_path(const NodePath& sim_node_path);

        float get_run_anim_vel() const;
        void set_run_anim_vel(float run_anim_vel);

        float get_splat_h_vel() const;
        void set_splat_h_vel(float splat_h_vel);

        bool get_is_running() const;
        void set_is_running(bool is_running);

        bool get_is_falling() const;
        void set_is_falling(bool is_falling);

        bool get_is_splat_h() const;
        void set_is_splat_h(bool is_splat_h);

        NodePath sim_node_path;
        float run_anim_vel;
        float splat_h_vel;
        bool is_running;
        bool is_falling;
        bool is_splat_h;
    };   
}

#endif // PLAYER_H