@echo off

if "%~1" == "" (
    pushd godot_sim
    scons
    popd
)
if "%~1" == "editor" (
    pushd godot
    scons
    popd
)
if "%~1" == "editor_debug" (
    pushd godot
    scons dev_build="yes"
    popd
)