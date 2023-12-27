extends AnimatedSprite2D

@export var sim: GodotSim;

func _process(_delta):
    position = Vector2(sim.get_player_pos())
    
