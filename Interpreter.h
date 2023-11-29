#ifndef INTERPRETER_H
#define INTERPRETER_H

//Max number of symbols in symbol table.
#define MAX_SYMBOLS 128

//Range floating point values must be within to be considered equal.
#define FLOAT_COMP_EPSILON 0.00001

//Max number of nested while loops.
#define MAX_NESTED_WHILE_LOOPS 128

//Max number of nested control blocks.
#define MAX_CONTROL_BLOCKS 256

/*
    Start the interpretation of a file.
*/
void cfg_start(
    const char* pFileName //C string to name of file (including extension).
    );

#endif