class_name Sim
extends GodotSim

func _notification(what: int):
    if what == NOTIFICATION_ENTER_TREE:
        init()
    if what == NOTIFICATION_EXIT_TREE or what == NOTIFICATION_WM_CLOSE_REQUEST:
        deinit()

func _process(delta: float):
    update(delta)
