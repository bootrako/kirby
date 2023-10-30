@echo off

if "%~1" == "" (
    set build_folder="godot_sim"
    set args=
)
if "%~1" == "editor" (
    set build_folder="godot"
    set args=
)
if "%~1" == "editor_debug" (
    set build_folder="godot"
    set args= dev_build="yes"
)

pushd %build_folder%
scons %args%
popd