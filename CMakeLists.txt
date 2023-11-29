cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_STANDARD 99)

project(CFGProject LANGUAGES C)

add_executable(CFG main.c Interpreter.c Token.c Interpreter.h Token.h)