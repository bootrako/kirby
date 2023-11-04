#ifndef GODOT_SIM_H
#define GODOT_SIM_H

#include <godot_cpp/classes/Node.hpp>
#include <kirby_sim.h>

namespace godot {

class KirbySim : public Node {
    GDCLASS(KirbySim, Node)
    
public:
    KirbySim();
    ~KirbySim();

protected:
    static void _bind_methods();

private:
    void _sim_init();
    void _sim_deinit();
    int _sim_update(float delta_time);
    Vector2i _sim_get_player_pos() const;

    static void* _godot_alloc(void* context, int size);
    static void _godot_free(void* context, void* ptr);
    static void _godot_panic(void* context, const char* err_msg);
    static void _godot_log(void* context, const char* msg);
    static int _godot_input_action_pressed(void* context, const kirby_sim_input_action input_action);
    static const char* _godot_open_data_file(void* context, kirby_sim_data_file data_file);

    kirby_sim* sim;
};

}

#endif // GODOT_SIM_H