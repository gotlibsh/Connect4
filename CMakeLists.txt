cmake_minimum_required(VERSION 3.0.0)
project(Connect4 VERSION 0.2.0)

IF(MSVC)
    add_compile_options(/wd4996)
ENDIF()

include_directories(include)

add_executable( Connect4
                src/main.c
                src/c4_utils.c
                src/evaluation.c
                src/minimax.c
                src/play_mode.c
                src/util_mode.c
                src/tester.c)
