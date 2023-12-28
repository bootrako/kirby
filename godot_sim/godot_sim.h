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

    Dictionary get_info() const;
    Dictionary get_cfg() const;

protected:
    static void _bind_methods();

private:
    void update_info();
    void set_cfg(Dictionary cfg);

    static void* _godot_alloc(void* ctx, int size);
    static void _godot_free(void* ctx, void* ptr);
    static void _godot_panic(void* ctx, char* err_msg);
    static void _godot_log(void* ctx, char* msg);
    static bool _godot_is_action_active(void* ctx, btk_action action);
    static char* _godot_read_data(void* ctx, btk_data data, int* out_len);
    static void _godot_read_cfg(void* ctx, btk_cfg* cfg);

    Dictionary info;
    Dictionary cfg;
    btk_sim* sim;
};

}

#endif // GODOT_SIM_H