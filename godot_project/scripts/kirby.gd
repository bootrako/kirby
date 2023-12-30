extends Sprite2D
class_name Kirby

@export var sim: GodotSim
@export var run_anim_vel: float

@onready var _info: Dictionary = sim.get_info()

@onready var is_running: bool = false
@onready var is_falling: bool = false

func _process(_delta: float):
    position = _info["player_pos"] as Vector2
    
    if Input.is_action_pressed("move_left") && !Input.is_action_pressed("move_right"):
        flip_h = true
    if Input.is_action_pressed("move_right") && !Input.is_action_pressed("move_left"):
        flip_h = false
        
    is_running = false
    is_falling = false
    if _info["player_is_grounded"] == true:
        var vel := _info["player_vel"] as Vector2
        is_running = absf(vel.x) > run_anim_vel
    else:
        is_falling = true
