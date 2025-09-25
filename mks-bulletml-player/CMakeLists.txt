cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-player LANGUAGES C)

# activate c11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# path to interpreter library
set(INTERPRETER_INCLUDE_DIR "../mks-bulletml-interpreter")
set(INTERPRETER_LIB_DIR "../build-mks-bulletml-interpreter-Win-Desktop-Debug")

if(WIN32)
    set(LIBXML2_ROOT "e:/libxml2_build")
    set(ICONV_ROOT "e:/iconv-1.14-win32-x86_64")

    set(RAYLIB_INCLUDE_DIR "e:/raylib_build/win_include")
    set(RAYLIB_LIB_DIR "e:/raylib_build/win_lib")

    link_directories("${LIBXML2_ROOT}/lib")
    link_directories("${ICONV_ROOT}/lib")
    link_directories(${RAYLIB_LIB_DIR})
else()
    # Prefer target_include_directories for modern CMake
    include_directories(
        ${INTERPRETER_INCLUDE_DIR}
        /usr/include/libxml2
    )
endif()

# include .so-file/.dll/.lib
link_directories(${INTERPRETER_LIB_DIR})

# create executable
add_executable(mks-bulletml-player
    main.c
    mksbmlp-defines.h
    app.c app.h
    utility.c utility.h
    user-interface.c user-interface.h
    dirent.h
)

# Modern way: set includes per target (uncomment if you want this style)
target_include_directories(mks-bulletml-player PRIVATE
    ${INTERPRETER_INCLUDE_DIR}
    $<$<BOOL:${WIN32}>:${RAYLIB_INCLUDE_DIR}>
    $<$<BOOL:${WIN32}>:${LIBXML2_ROOT}/include>
    $<$<BOOL:${WIN32}>:${ICONV_ROOT}/include>
    $<$<NOT:$<BOOL:${WIN32}>>:/usr/include/libxml2>
)

if(WIN32)
    target_link_libraries(mks-bulletml-player
        mks-bulletml-interpreter
        raylib
        libxml2
    )
    # Copy DLLs to output dir after build
    add_custom_command(TARGET mks-bulletml-player POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${INTERPRETER_LIB_DIR}/mks-bulletml-interpreter.dll"
            "$<TARGET_FILE_DIR:mks-bulletml-player>/mks-bulletml-interpreter.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RAYLIB_LIB_DIR}/raylib.dll"
            "$<TARGET_FILE_DIR:mks-bulletml-player>/raylib.dll"
    )
else()
    target_link_libraries(mks-bulletml-player
        mks-bulletml-interpreter
        raylib
        xml2
        m
    )
endif()
