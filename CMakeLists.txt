cmake_minimum_required(VERSION 3.0.0)
project(Connect4 VERSION 0.2.0)

IF(MSVC)
    add_compile_options(/wd4996)
ENDIF()

add_executable( Connect4
                main.c
                c4_utils.c
                evaluation.c
                minimax.c
                play_mode.c
                util_mode.c
                tester.c)
