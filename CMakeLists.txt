cmake_minimum_required(VERSION 3.26)

project(CFGProject LANGUAGES C)

add_executable(CFG main.c Interpreter.h Token.h)