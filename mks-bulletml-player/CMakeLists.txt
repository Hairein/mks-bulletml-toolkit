cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-player LANGUAGES C)

# activate c11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# path to interpreter library
set(INTERPRETER_INCLUDE_DIR "../mks-bulletml-interpreter")
set(INTERPRETER_LIB_DIR "../build-mks-bulletml-interpreter-Desktop-Debug")

# include headers
include_directories(
    ${INTERPRETER_INCLUDE_DIR}
    /usr/include/libxml2
)

# include .so-file
set(RAYLIB_LIB_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Desktop-Debug")
link_directories(${RAYLIB_LIB_DIR})
link_directories(${INTERPRETER_LIB_DIR})

# create executable
add_executable(mks-bulletml-player
    main.c
    raylib.h raygui.h raymath.h
    app.c app.h
    utility.h
    utility.c
)

# link shared library
target_link_libraries(mks-bulletml-player
    mks-bulletml-interpreter
    raylib
    m
)
