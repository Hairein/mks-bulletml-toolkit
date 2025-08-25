cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-interpreter LANGUAGES C)

# C11 aktivieren
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# create library
add_library(mks-bulletml-interpreter SHARED
    mks-bulletml-interpreter.c
)

# include headers
target_include_directories(mks-bulletml-interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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
