class_name Kirby
extends KirbySim

func _ready():
    sim_init()
    
func _process(delta_time: float):
    sim_update(delta_time)
