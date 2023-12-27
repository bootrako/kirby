extends AnimatedSprite2D
class_name Kirby

@export var sim: GodotSim;

func _process(_delta):
    var info := sim.get_info()
    position = info["player_pos"] as Vector2
