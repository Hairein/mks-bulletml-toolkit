cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-player LANGUAGES C)

# Activate C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Paths to interpreter library
set(INTERPRETER_INCLUDE_DIR "../mks-bulletml-interpreter")
set(INTERPRETER_LIB_DIR "../build-mks-bulletml-interpreter-Win-Desktop-Debug")

if(WIN32)
    # Use environment variable if set, otherwise fallback to default
    if(DEFINED ENV{LIBXML2_ROOT})
        set(LIBXML2_ROOT $ENV{LIBXML2_ROOT})
    else()
        set(LIBXML2_ROOT "e:/libxml2_build")
    endif()

    if(DEFINED ENV{ICONV_ROOT})
        set(ICONV_ROOT $ENV{ICONV_ROOT})
    else()
        set(ICONV_ROOT "e:/iconv-1.14-win32-x86_64")
    endif()

    if(DEFINED ENV{RAYLIB_INCLUDE_DIR})
        set(RAYLIB_INCLUDE_DIR $ENV{RAYLIB_INCLUDE_DIR})
    else()
        set(RAYLIB_INCLUDE_DIR "e:/raylib_build/win_include")
    endif()

    if(DEFINED ENV{RAYLIB_LIB_DIR})
        set(RAYLIB_LIB_DIR $ENV{RAYLIB_LIB_DIR})
    else()
        set(RAYLIB_LIB_DIR "e:/raylib_build/win_lib")
    endif()

    link_directories("${LIBXML2_ROOT}/lib")
    link_directories("${ICONV_ROOT}/lib")
    link_directories(${RAYLIB_LIB_DIR})
endif()

link_directories(${INTERPRETER_LIB_DIR})

# Source and header files
set(SRC_FILES
    main.c
    app.c
    utility.c
    user-interface.c
)

set(HEADER_FILES
    mksbmlp-defines.h
    app.h
    utility.h
    user-interface.h
)

if(WIN32)
    list(APPEND HEADER_FILES dirent.h)
endif()

add_executable(mks-bulletml-player ${SRC_FILES})

# Make header files visible in IDE (not used for compiling)
source_group("Header Files" FILES ${HEADER_FILES})

# Modern way: set includes per target
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
