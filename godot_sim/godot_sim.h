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

	static void* _godot_alloc(void* context, int size);
    static void _godot_free(void* context, void* ptr);

	kirby_sim* sim;
};

}

#endif // GODOT_SIM_H