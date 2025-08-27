cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-interpreter LANGUAGES C)

# activate c11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# create library
add_library(mks-bulletml-interpreter SHARED
    mks-bulletml-interpreter.c mks-bulletml-interpreter.h
    raylib.h raymath.h
    tinyexpr.c tinyexpr.h
    platform_random.h
    playback.c playback.h
    virtual_bullet.c virtual_bullet.h
    state.c state.h
    utility.c utility.h
    bulletml_base.c bulletml_base.h
    bulletml.c bulletml.h
    bullet.c bullet.h
    action.c action.h
    fire.c fire.h
    change_direction.c change_direction.h
    change_speed.c change_speed.h
    accel.c accel.h
    wait.c wait.h
    vanish.c vanish.h
    repeat.c repeat.h
    direction.c direction.h
    speed.c speed.h
)

# include headers
target_include_directories(mks-bulletml-interpreter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    /usr/include/libxml2
)

# target directory of the .so-file
set(PLAYER_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Desktop-Debug")

# copy the .so-file after the build
add_custom_command(TARGET mks-bulletml-interpreter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PLAYER_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:mks-bulletml-interpreter>
        ${PLAYER_BUILD_DIR}
    COMMENT "Copy libmks-bulletml-interpreter.so to build-mks-bulletml-player-Desktop-Debug"
)

# link libraries
target_link_libraries(mks-bulletml-interpreter
    xml2
    m
)
