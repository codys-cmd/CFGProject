#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Token.h"

//Max number of symbols in symbol table.
#define MAX_SYMBOLS 128

//Range floating point values must be within to be considered equal.
#define FLOAT_COMP_EPSILON 0.00001

//Max number of nested while loops.
#define MAX_NESTED_WHILE_LOOPS 128

//Max number of nested control blocks.
#define MAX_CONTROL_BLOCKS 256

/*
    Structure which contains values.
*/
typedef union {
    long long integer;
    bool boolean;
    double floating_point;
} Value;

/*
    Represents the possible types a value or symbol may be.
*/
typedef enum {
    Integer, Boolean, Float, Uninitialized
} Type;

/*
    Maps defined symbols to their corresponding types
    and values via their indicies.
*/
typedef struct {
    
    int length;
    Character* symbolTokens [MAX_SYMBOLS];
    Type types [MAX_SYMBOLS];
    Value values [MAX_SYMBOLS];

} SymbolTable;

/*
    Prints information from the symbol table,
    including symbol names, and their corresponding
    types and values.
*/
void cfg_printSymbolTable(
    SymbolTable* pTable //Pointer to symbol table.
    );

/*
    Interprets a c-style string which represents a series of valid statements.
    Runs until it reaches a null character.
*/
void cfg_interpretStatements(
    char* pBuffer,      //Pointer to the string which represents the statements.
    SymbolTable* pTable //Current symbol table.
    );

/*
    Start the interpretation of a file.
*/
void cfg_interpretFile(
    const char* pFileName, //C string to name of file (including extension).
    SymbolTable* pTable    //Symbol table to use when interpreting file.
    );

#endif