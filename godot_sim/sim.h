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
    Dictionary get_cfg() const;
    void set_cfg(Dictionary cfg);

    static void* _alloc(void* ctx, int size);
    static void _free(void* ctx, void* ptr);
    static void _panic(void* ctx, char* err_msg);
    static void _log(void* ctx, char* msg);
    static bool _is_action_pressed(void* ctx, btk_action action);
    static char* _read_data(void* ctx, btk_data data, int* out_len);
    static void _read_cfg(void* ctx, btk_cfg* cfg);

    Dictionary cfg;
    btk_sim* sim;
};

}

#endif // SIM_H