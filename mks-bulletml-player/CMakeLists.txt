cmake_minimum_required(VERSION 3.5)

project(mks-bulletml-player LANGUAGES C)

# C11 aktivieren
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# path to interpreter library
set(INTERPRETER_INCLUDE_DIR "../mks-bulletml-interpreter")
set(INTERPRETER_LIB_DIR "../build-mks-bulletml-interpreter-Desktop-Debug")

# Header einbinden
include_directories(${INTERPRETER_INCLUDE_DIR})

# .so-Datei einbinden
link_directories(${INTERPRETER_LIB_DIR})

# Executable erstellen
add_executable(mks-bulletml-player
    main.c
)

# Mit der Shared Library linken
target_link_libraries(mks-bulletml-player
    mks-bulletml-interpreter
)
