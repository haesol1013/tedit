cmake_minimum_required(VERSION 3.30)
project(tedit C)

set(CMAKE_C_STANDARD 11)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/pdcurses
)

set(
        SOURCES
        src/main.c
        src/file_io.c
        src/editor.c
)

add_executable(tedit ${SOURCES})

target_link_libraries(tedit
        ${PROJECT_SOURCE_DIR}/external/pdcurses/pdcurses.a
)

set_target_properties(tedit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
