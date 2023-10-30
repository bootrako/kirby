@echo off

if "%~1" == "" (
    set args=custom_modules="../"
)
if "%~1" == "editor" (
    set args=custom_modules="../"
)
if "%~1" == "editor_debug" (
    set args=custom_modules="../" dev_build="yes"
)

pushd godot
scons %args%
popd