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

protected:
    static void _bind_methods();

private:
    void _sim_update(float delta_time);
    Vector2i _sim_get_player_pos() const;

    static void* _godot_alloc(void* context, int size);
    static void _godot_free(void* context, void* ptr);
    static void _godot_panic(void* context, const char* err_msg);
    static void _godot_log(void* context, const char* msg);
    static bool _godot_is_action_active(void* context, btk_action action);
    static const char* _godot_read_data(void* context, btk_data data, int* out_len);

    btk_sim* sim = nullptr;
};

}

#endif // GODOT_SIM_H