cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-interpreter LANGUAGES C)

# activate c11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# create library
add_library(mks-bulletml-interpreter SHARED
    mks-bulletml-interpreter.c mks-bulletml-interpreter.h
    tinyexpr.c tinyexpr.h
    platform-random.h
    playback.c playback.h
    virtual_bullet.c virtual_bullet.h
    state.c state.h
    utility.c utility.h
    bulletml-base.c bulletml-base.h
    bulletml.c bulletml.h
    bullet.c bullet.h
    action.c action.h
    fire.c fire.h
    change-direction.c change-direction.h
    change-speed.c change-speed.h
    accel.c accel.h
    wait.c wait.h
    vanish.c vanish.h
    repeat.c repeat.h
    direction.c direction.h
    speed.c speed.h
    horizontal.c horizontal.h
    vertical.c vertical.h
    term.c term.h
    times.c times.h
    bullet-ref.c bullet-ref.h
    action-ref.c action-ref.h
    fire-ref.c fire-ref.h
    param.c param.h
    bml-number.c bml-number.h
    mksbmli-defines.h
    interpreter.c interpreter.h
    virtual_bullet_manager.c virtual_bullet_manager.h
    action_info_block.c action_info_block.h
    Vector2D.c Vector2D.h
)

# Conditional libxml2 include and library paths
if(WIN32)
    set(LIBXML2_ROOT "e:/libxml2_build")
    set(ICONV_ROOT "e:/iconv-1.14-win32-x86_64")
    target_include_directories(mks-bulletml-interpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        "${LIBXML2_ROOT}/include"
        "${ICONV_ROOT}/include"
    )
    target_link_directories(mks-bulletml-interpreter PUBLIC
        "${LIBXML2_ROOT}/lib"
        "${ICONV_ROOT}/lib"
    )
    target_link_libraries(mks-bulletml-interpreter
        libxml2
    )
else()
    target_include_directories(mks-bulletml-interpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        /usr/include/libxml2
    )
    target_link_libraries(mks-bulletml-interpreter
        xml2
        m
    )
endif()

# Set PLAYER_BUILD_DIR and copy target based on Debug or Release
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(PLAYER_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Win-Desktop-Debug")
        set(COPY_COMMENT "Copy libmks-bulletml-interpreter.so to build-mks-bulletml-player-Win-Desktop-Debug")
    else()
        set(PLAYER_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Win-Desktop-Release")
        set(COPY_COMMENT "Copy libmks-bulletml-interpreter.so to build-mks-bulletml-player-Win-Desktop-Release")
    endif()
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(PLAYER_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Desktop-Debug")
        set(COPY_COMMENT "Copy libmks-bulletml-interpreter.so to build-mks-bulletml-player-Desktop-Debug")
    else()
        set(PLAYER_BUILD_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Desktop-Release")
        set(COPY_COMMENT "Copy libmks-bulletml-interpreter.so to build-mks-bulletml-player-Desktop-Release")
    endif()
endif()

add_custom_command(TARGET mks-bulletml-interpreter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PLAYER_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:mks-bulletml-interpreter>
        ${PLAYER_BUILD_DIR}
    COMMENT "${COPY_COMMENT}"
)
