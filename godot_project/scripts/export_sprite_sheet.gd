@tool
extends EditorScript

func _run() -> void:
    var sprite_sheet_path := "res://textures/t_kirby_sprite.png"
    var json_path := "res://.godot/bootrako/t_kirby_sprite.json"
    var args: Array[String] = [
        "-b",
        ProjectSettings.globalize_path("res://../aseprite/kirby_small.aseprite"),
        ProjectSettings.globalize_path("res://../aseprite/star_small.aseprite"),
        "--sheet",
        ProjectSettings.globalize_path(sprite_sheet_path),
        "--data",
        ProjectSettings.globalize_path(json_path),
        "--sheet-pack",
        "--inner-padding",
        "1",
        "--format",
        "json-array",
        "--filename-format",
        "{title}.{tag}.{tagframe}",
    ]
    
    var output: Array[String] = []
    var exit_code := OS.execute("aseprite", args, output, true, true)
    
    if (exit_code == -1 or output != [""]):
        push_error("aseprite command failed: ", output)
        
    if ResourceLoader.exists(sprite_sheet_path):
        EditorInterface.get_resource_filesystem().scan()
    else:
        EditorInterface.get_resource_filesystem().update_file(sprite_sheet_path)    
        EditorInterface.get_resource_filesystem().reimport_files([sprite_sheet_path])
        
    var json_string := FileAccess.get_file_as_string(json_path)
    if json_string == null:
        push_error("failed to open json file " + json_path, FileAccess.get_open_error())
    
    var json := JSON.parse_string(json_string) as Dictionary
    if json == null:
        push_error("failed to parse json for file " + json_path)
    
    var sprite_sheet_tex := ResourceLoader.load(sprite_sheet_path, "Image", ResourceLoader.CACHE_MODE_IGNORE) as Texture2D
    sprite_sheet_tex.take_over_path(sprite_sheet_path)
    
    const scene_name = "kirby_anim_player"
    var anim_player := AnimationPlayer.new()
    anim_player.name = scene_name
    const anim_library_name := ""
    if !anim_player.has_animation_library(anim_library_name):
        anim_player.add_animation_library(anim_library_name, AnimationLibrary.new())
    var anim_library := anim_player.get_animation_library(anim_library_name)
    
    var anim_lengths := Dictionary()
        
    for json_sprite in (json["frames"] as Array):
        var region := Rect2(json_sprite.frame.x + ((json_sprite.frame.w - json_sprite.sourceSize.w) / 2), json_sprite.frame.y + ((json_sprite.frame.h - json_sprite.sourceSize.h) / 2), json_sprite.sourceSize.w, json_sprite.sourceSize.h)
        var filename_ids := (json_sprite.filename as String).split(".")
        var name := filename_ids[0]
        var tag := filename_ids[1]
        
        if tag.is_empty():
            var atlas_tex_path := "res://textures/at_" + name + ".tres"
            var atlas_tex := AtlasTexture.new()
            atlas_tex.atlas = sprite_sheet_tex
            atlas_tex.region = region
            atlas_tex.filter_clip = true
            atlas_tex.take_over_path(atlas_tex_path)
     
            if ResourceSaver.save(atlas_tex, atlas_tex_path) != OK:
                push_error("failed to save atlas texture " + atlas_tex_path)
        else:
            if !anim_library.has_animation(tag):
                var new_anim := Animation.new()
                new_anim.length = 0
                new_anim.resource_name = tag
                if (tag.ends_with("_loop")):
                    new_anim.loop_mode = Animation.LOOP_LINEAR
                anim_library.add_animation(tag, new_anim)
                anim_lengths[tag] = 0

            const anim_track_path := ".:region_rect"
            var anim := anim_library.get_animation(tag)
            var anim_track_index := anim.find_track(anim_track_path, Animation.TYPE_VALUE)
            if anim_track_index == -1:
                anim_track_index = anim.add_track(Animation.TYPE_VALUE)
                anim.track_set_path(anim_track_index, anim_track_path)
                anim.value_track_set_update_mode(anim_track_index, Animation.UPDATE_DISCRETE)
            anim.track_insert_key(anim_track_index, anim_lengths[tag], region)
            anim_lengths[tag] += json_sprite.duration / 1000
            anim.length = anim_lengths[tag]
    
    const scene_path := "res://scenes/s_" + scene_name + ".tscn"
    var scene := PackedScene.new()
    var result := scene.pack(anim_player)
    if result != OK:
        push_error("failed to pack scene: " + scene_name)
    scene.take_over_path(scene_path)
    if ResourceSaver.save(scene, scene_path) != OK:
        push_error("failed to save scene " + scene_path)
    
    print("successfully exported sprite sheet")
