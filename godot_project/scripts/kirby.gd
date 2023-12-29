extends AnimatedSprite2D
class_name Kirby

@export var sim: GodotSim
@export var run_anim_vel: float

func _process(_delta: float):
    var info := sim.get_info()
    position = info["player_pos"] as Vector2
    
    if Input.is_action_pressed("move_left") && !Input.is_action_pressed("move_right"):
        flip_h = true
    if Input.is_action_pressed("move_right") && !Input.is_action_pressed("move_left"):
        flip_h = false
    
    if info["player_is_grounded"] == true:
        var vel := info["player_vel"] as Vector2
        if (absf(vel.x) > run_anim_vel):
            _play_anim("run")
        else:
            _play_anim("idle")
    else:
        _play_anim("fall")
        
    if info["event_player_landed"]["count"] > 0:
        print("landed!")

func _play_anim(anim_name: StringName):
    if animation != anim_name:
        play(anim_name)
