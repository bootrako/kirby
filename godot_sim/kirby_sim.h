#ifndef KIRBY_SIM_H
#define KIRBY_SIM_H

#include <godot_cpp/classes/Node.hpp>

namespace godot {

class KirbySim : public Node {
	GDCLASS(KirbySim, Node)
	
public:
	KirbySim();
	~KirbySim();

protected:
	static void _bind_methods();
};

}

#endif // KIRBY_SIM_H