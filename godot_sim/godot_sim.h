#ifndef GODOT_SIM_H
#define GODOT_SIM_H

#include <godot_cpp/classes/Node.hpp>
#include <btk_sim.h>

namespace godot {

class GodotSim : public Node {
    GDCLASS(GodotSim, Node)
    
public:
    GodotSim();
    ~GodotSim();

    void _process(double delta);

    Vector2 get_player_pos() const;
    Vector2 get_player_vel() const;
    
    void set_player_accel(Vector2 player_accel);
    Vector2 get_player_accel() const;

    void set_player_vel_max(Vector2 player_vel_max);
    Vector2 get_player_vel_max() const;

    void set_player_vel_damp_x(float player_vel_damp_x);
    float get_player_vel_damp_x() const;

    void set_player_gravity(float player_gravity);
    float get_player_gravity() const;

    void set_player_max_jump_timer(float player_max_jump_timer);
    float get_player_max_jump_timer() const;

protected:
    static void _bind_methods();

private:
    static void* _godot_alloc(void* ctx, int size);
    static void _godot_free(void* ctx, void* ptr);
    static void _godot_panic(void* ctx, char* err_msg);
    static void _godot_log(void* ctx, char* msg);
    static bool _godot_is_action_active(void* ctx, btk_action action);
    static char* _godot_read_data(void* ctx, btk_data data, int* out_len);
    static void _godot_read_cfg(void* ctx, btk_cfg* cfg);

    btk_cfg cfg;
    btk_sim* sim;
};

}

#endif // GODOT_SIM_H