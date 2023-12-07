#include <stdio.h>
#include <stdlib.h>

#include "Interpreter.h"

int main() {

    SymbolTable* table = (SymbolTable*) malloc(sizeof(SymbolTable));
    table->length = 0;
    table->symbolTokens[0] = NULL;

    int testsPassed = 0;

    printf("Starting Test 1...\n");
    cfg_interpretStatements(
        "a = 5;",
        table
    );

    if (
        table->length == 1 &&
        table->types[0] == Integer &&
        table->values[0].integer == 5
    ) {
        printf("Test #1 Passed!\n");
        testsPassed++;
    } 

    printf("Starting Test 2...\n");
    cfg_interpretStatements(
        "x = 5; if (x < 10) { y = 20; } else { y = 30; }",
        table
    );

    if (
        table->length == 3 &&
        table->types[2] == Integer &&
        table->values[2].integer == 20
    ) {
        printf("Test #2 Passed!\n");
        testsPassed++;
    } 

    printf("Starting Test 3...\n");
    cfg_interpretStatements(
        "i = 0; while (i < 5) { i = i + 1; }",
        table
    );

    if (
        table->length == 4 &&
        table->types[3] == Integer &&
        table->values[3].integer == 5
    ) {
        printf("Test #3 Passed!\n");
        testsPassed++;
    } 

    printf("Starting Test 4...\n");
    cfg_interpretStatements(
        "a = -1; if (a == 1) { b = 2; } else { b = 3; }",
        table
    );

    if (
        table->length == 5 &&
        table->types[4] == Integer &&
        table->values[4].integer == 3
    ) {
        printf("Test #4 Passed!\n");
        testsPassed++;
    } 

    printf("Starting Test 5...\n");
    cfg_interpretStatements(
        "c = (true || false) && (false && true);",
        table
    );

    if (
        table->length == 6 &&
        table->types[5] == Boolean &&
        table->values[5].boolean == False
    ) {
        printf("Test #5 Passed!\n");
        testsPassed++;
    } 

    printf("Starting Test 6...\n");
    cfg_interpretStatements(
        "c = 16 + -2 ^ (-3 + 5 + -1 + 1);",
        table
    );

    if (
        table->length == 6 &&
        table->types[5] == Integer &&
        table->values[5].integer == 20
    ) {
        printf("Test #6 Passed!\n");
        testsPassed++;
    }

    printf("Starting Test 7...\n");
    cfg_interpretStatements(
        "c = 2 ^ (-3 ^ (5 - 3));",
        table
    );

    if (
        table->length == 6 &&
        table->types[5] == Integer &&
        table->values[5].integer == 512
    ) {
        printf("Test #7 Passed!\n");
        testsPassed++;
    }

    printf("%d / 7 Tests Passed. Press enter to close.", testsPassed);
    char input;
    scanf("%c", &input);

}