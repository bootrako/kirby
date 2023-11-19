@echo off

if "%~1" == "" (
    pushd godot_sim
    scons -Q dev_build="yes"
    popd
)
if "%~1" == "release" (
    pushd godot_sim
    scons -Q target=template_release
    popd
)
if "%~1" == "editor" (
    pushd godot
    scons -Q
    popd
)
if "%~1" == "editor_debug" (
    pushd godot
    scons -Q dev_build="yes"
    popd
)