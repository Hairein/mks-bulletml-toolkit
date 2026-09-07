cmake_minimum_required(VERSION 3.5)
project(mks-bulletml-player LANGUAGES C)

# Activate C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Default build type for single-config generators (Makefiles/Ninja) if unset
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Paths to interpreter library
set(INTERPRETER_INCLUDE_DIR "../mks-bulletml-interpreter")
# $<CONFIG> resolves to Debug/Release per-build (works for both single- and multi-config generators)
set(INTERPRETER_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../mks-bulletml-interpreter/build/Desktop-$<CONFIG>")

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

add_executable(mks-bulletml-player ${SRC_FILES}
    app.h mksbmlp-defines.h user-interface.h utility.h)

# Make header files visible in IDE (not used for compiling)
source_group("Header Files" FILES ${HEADER_FILES})

# Explicit output location: build/Desktop-Debug or build/Desktop-Release.
# The RUNTIME_OUTPUT_DIRECTORY_<CONFIG> properties are required for multi-config
# generators (VS, Xcode, Ninja Multi-Config) — otherwise CMake auto-appends its
# own \Debug or \Release subfolder on top of whatever RUNTIME_OUTPUT_DIRECTORY says.
set_target_properties(mks-bulletml-player PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/Desktop-$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/Desktop-Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/Desktop-Release"
)

# Interpreter lib dir is config-dependent, so it's scoped to this target
# with a generator expression rather than a global link_directories() call
target_link_directories(mks-bulletml-player PRIVATE
    ${INTERPRETER_LIB_DIR}
)

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

    # Deploy folder: a sibling of the project root, one level above the source dir,
    # e.g. ..\build-mks-bulletml-player-Win-Desktop-Release
    set(DEPLOY_DIR "${CMAKE_SOURCE_DIR}/../build-mks-bulletml-player-Win-Desktop-$<CONFIG>")

    # Copy the exe and every dependent DLL to both the normal build output dir
    # (for raylib.dll, needed to run/debug from build/Desktop-$<CONFIG> directly)
    # and to DEPLOY_DIR (a standalone, runnable copy).
    add_custom_command(TARGET mks-bulletml-player POST_BUILD
        # Keep raylib.dll next to the exe in the regular build output too,
        # so Run/Debug from Qt Creator still works without the deploy step.
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RAYLIB_LIB_DIR}/raylib.dll"
            "$<TARGET_FILE_DIR:mks-bulletml-player>/raylib.dll"

        # Make sure the deploy folder exists
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_DIR}"

        # Copy the player exe
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:mks-bulletml-player>"
            "${DEPLOY_DIR}"

        # Copy dependent DLLs: interpreter, raylib, libxml2, iconv
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${INTERPRETER_LIB_DIR}/mks-bulletml-interpreter.dll"
            "${DEPLOY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RAYLIB_LIB_DIR}/raylib.dll"
            "${DEPLOY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIBXML2_ROOT}/lib/libxml2.dll"
            "${DEPLOY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ICONV_ROOT}/bin/libiconv-2.dll"
            "${DEPLOY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ICONV_ROOT}/bin/libcharset-1.dll"
            "${DEPLOY_DIR}"

        COMMENT "Deploying mks-bulletml-player and dependent DLLs to ${DEPLOY_DIR}"
    )
else()
    target_link_libraries(mks-bulletml-player
        mks-bulletml-interpreter
        raylib
        xml2
        m
    )
endif()
