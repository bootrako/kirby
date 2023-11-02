@echo off

if "%~1" == "" (
    pushd sim
    zig build -Dtarget=native-native-msvc || goto :error
    popd
    pushd godot_sim
    scons dev_build="yes"
    popd
)
if "%~1" == "release" (
    pushd sim
    zig build -Dtarget=native-native-msvc -Doptimize=ReleaseFast || goto :error
    popd
    pushd godot_sim
    scons target=template_release
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
goto :EOF

:error
popd
exit /b