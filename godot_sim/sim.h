#ifndef SIM_H
#define SIM_H

#include <godot_cpp/classes/Node.hpp>
#include <btk_sim.h>

namespace godot {

class Sim : public Node {
    GDCLASS(Sim, Node)
    
public:
    Sim();
    ~Sim();

    void _process(double delta);

    btk_sim* get_sim() const;

protected:
    static void _bind_methods();

private:
    static void* _alloc(void* ctx, int size);
    static void _free(void* ctx, void* ptr);
    static void _panic(void* ctx, char* err_msg);
    static void _log(void* ctx, char* msg);
    static bool _is_action_pressed(void* ctx, btk_action action);
    static char* _read_data(void* ctx, btk_data data, int* out_len);
    static void _read_cfg(void* ctx, btk_cfg* cfg);

    Vector2 get_player_accel() const;
    void set_player_accel(const Vector2& player_accel);

    Vector2 get_player_vel_min() const;
    void set_player_vel_min(const Vector2& player_vel_min);

    Vector2 get_player_vel_max() const;
    void set_player_vel_max(const Vector2& player_vel_max);

    float get_player_vel_damp_x() const;
    void set_player_vel_damp_x(float player_vel_damp_x);

    float get_player_jump_release_vel_y() const;
    void set_player_jump_release_vel_y(float player_jump_release_vel_y);

    float get_player_gravity() const;
    void set_player_gravity(float player_gravity);

    float get_player_gravity_dive_stunned() const;
    void set_player_gravity_dive_stunned(float player_gravity_dive_stunned);

    float get_player_max_jump_timer() const;
    void set_player_max_jump_timer(float player_max_jump_timer);

    float get_player_fall_dive_timer() const;
    void set_player_fall_dive_timer(float player_fall_dive_timer);

    float get_player_dive_bounce_vel_y() const;
    void set_player_dive_bounce_vel_y(float player_dive_bounce_vel_y);

    btk_cfg cfg;
    btk_sim* sim;
};

}

#endif // SIM_H