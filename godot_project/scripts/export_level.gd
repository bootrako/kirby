@tool
extends EditorScript

func find_child_by_class(node: Node, type: String) -> Node:
    var nodes := node.find_children("*", type)
    if nodes.size() != 1:
        return null
    else:
        return nodes[0]

func _run() -> void:
    var scene := get_scene()
    print("exporting scene: " + scene.name)
    
    var out_data := ""
    
    var tilemap := find_child_by_class(scene, "TileMap") as TileMap
    if tilemap == null:
        push_error("failed to find tilemap node")
        return
    var map_size := tilemap.get_used_rect().size
    out_data += str(map_size.x) + "\n"
    out_data += str(map_size.y) + "\n"
    
    const collision_layer_name := "collision"
    const invalid_layer := -1
    var collision_layer := invalid_layer
    for i in tilemap.get_layers_count():
        if tilemap.get_layer_name(i) == collision_layer_name:
            collision_layer = i
    if collision_layer == invalid_layer:
        push_error("failed to find layer named " + collision_layer_name)
        return

    for y in map_size.y:
        for x in map_size.x:
            if tilemap.get_cell_tile_data(collision_layer, Vector2i(x, y)) != null:
                out_data += "1"
            else:
                out_data += "0"
        out_data += "\n"

    var player := find_child_by_class(scene, "Player") as Player
    if player == null:
        push_error("failed to find player node")
        return
    
    var tile_set := tilemap.tile_set
    if tile_set == null:
        push_error("tilemap has no tileset assigned")
        return
    var player_cell := Vector2i(player.position) / tile_set.tile_size
    out_data += str(player_cell.x) + "\n"
    out_data += str(player_cell.y) + "\n"
    
    var data_file_name := "res://data/" + scene.name + ".txt"
    var data_file: FileAccess = FileAccess.open(data_file_name, FileAccess.WRITE)
    data_file.store_string(out_data)
    
    print("successfully wrote file " + data_file_name)
