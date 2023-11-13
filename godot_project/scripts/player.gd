class_name Player
extends Sprite2D

@export var sim: GodotSim;

func _process(_delta):
    position = Vector2(sim.get_player_pos())
