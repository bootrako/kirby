class_name Player
extends Sprite2D

@export var kirby: Kirby;

func _process(_delta):
    position = Vector2(kirby.sim_get_player_pos())
