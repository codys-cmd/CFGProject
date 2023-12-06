cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_STANDARD 99)

project(CFGProject LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<IF:$<CONFIG:Debug>,Debug,Release>)

add_executable(Repl Repl.c Interpreter.c Token.c Interpreter.h Token.h)

add_executable(Tests Tests.c Interpreter.c Token.c Interpreter.h Token.h)

#Copies example file to output directory.
add_custom_command(TARGET Repl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/example.itr ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)