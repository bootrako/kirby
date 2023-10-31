#ifndef GODOT_SIM_H
#define GODOT_SIM_H

#include <godot_cpp/classes/Node.hpp>

namespace godot {

class KirbySim : public Node {
	GDCLASS(KirbySim, Node)
	
public:
	KirbySim();
	~KirbySim();

protected:
	static void _bind_methods();

private:
	int _get_number();
};

}

#endif // GODOT_SIM_H