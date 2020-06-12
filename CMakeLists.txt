cmake_minimum_required(VERSION 3.0.0)
project(Connect4 VERSION 0.1.0)

add_executable(Connect4 main.c c4_utils.c evaluation.c tester.c)