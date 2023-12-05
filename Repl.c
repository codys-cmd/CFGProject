#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include "Interpreter.h"

#define INPUT_BUFFER_LENGTH 256

//Thx stack overflow <3 xo xo
#define TO_STRING(x) Stringify(x)
    #define Stringify(x) #x

char inputBuffer[INPUT_BUFFER_LENGTH];

//REPL Keywords
const char* HELP_KW = "-help";
const char* QUIT_KW = "-q";
const char* SYMBOLS_KW = "-symbols";
const char* RUNFILE_KW = "-runfile";
const char* USE_REPL_TABLE_KW = "s";
const char* PRINT_TABLE_KW = "p";
const char* RUN_KW = "-run";
const char* DEL_SYMBOLS_KW = "-del_symbols";

//Repl's symbol table.
SymbolTable* symbolTable;

int main() {
    printf("Repl Started!\n");
    printf("Type \"-help\" for more information about how to use.\n");
    printf("Type \"-q\" to exit.\n");

    symbolTable = (SymbolTable*) malloc(sizeof(SymbolTable));
    symbolTable->length = 0;
    symbolTable->symbolTokens[0] = NULL;

    bool isRunning = True;
    while (isRunning) {
        printf("\n:");
        Token* token = (Token*) malloc( sizeof(Token) );
        scanf(" %"TO_STRING(INPUT_BUFFER_LENGTH)"[^\n]s", inputBuffer);
        char* ptr = cfg_getNextToken(inputBuffer, token, "", "+");
        printf("\n");

        //"-q" - Quit repl.
        if (cfg_isTokenKeyword(token->head, QUIT_KW)) {
            printf("Quitting Repl!\n");
            isRunning = False;
        
        //"-help" - Prints information about commands you can run.
        } else if (cfg_isTokenKeyword(token->head, HELP_KW)) {
            printf("Commands:\n");
            printf("\"-q\" - Quits the Repl.\n");
            printf("\"-symbols\" - Prints out the current contents of the repl's symbol table.\n");
            printf("\"-runfile FLAGS +FILE_NAME\" - Runs the file with that name.\n");
            printf("Flags:\n");
            printf("\t\"+s\" - Use current Repl's signal table, instead of instantiating a new one.\n");
            printf("\t\"+p\" - Print symbol table after execution.\n");
            printf("\"-run FLAGS +STATEMENTS\" - Interprets supplied statements, symbols stored in repl's table.\n");
            printf("Flags:\n");
            printf("\t\"+p\" - Print symbol table after execution.\n");
            printf("\"-del_symbols\" - Clears the symbol table.");

        //"-symbols" - Prints out the symbol table.
        } else if (cfg_isTokenKeyword(token->head, SYMBOLS_KW)) {
            cfg_printSymbolTable(symbolTable);
        
        //"-del_symbols" - Clears the symbol table.
        } else if (cfg_isTokenKeyword(token->head, DEL_SYMBOLS_KW)) {

            cfg_deleteSymbolTable(symbolTable);

            symbolTable = (SymbolTable*) malloc(sizeof(SymbolTable));
            symbolTable->length = 0;
            symbolTable->symbolTokens[0] = NULL;
        
        //"-runfile FLAGS +FILE_NAME" - Runs a file.
        } else if (cfg_tokenStartsWith(token->head, RUNFILE_KW)) {

            bool useReplTable = False;
            bool printTable = False;

            //Check for flags.
            while (token->terminatingChar == '+') {
                cfg_deleteToken(token);
                token = (Token*) malloc( sizeof(Token) );
                ptr = cfg_getNextToken(ptr, token, "", "+");
                if (!useReplTable) {
                    useReplTable = cfg_isTokenKeyword(token->head, USE_REPL_TABLE_KW);
                } 
                if (!printTable) {
                    printTable = cfg_isTokenKeyword(token->head, PRINT_TABLE_KW);
                }
            }

            char* fileName = cfg_tokenToCString( token->head );  
            
            SymbolTable* table;
            if (useReplTable) {
                table = symbolTable;
            } else {
                table = (SymbolTable*) malloc(sizeof(SymbolTable));
                table->length = 0;
                table->symbolTokens[0] = NULL;
            }

            cfg_interpretFile(fileName, table);

            if (printTable)
                cfg_printSymbolTable(table);
            if (!useReplTable)
                cfg_deleteSymbolTable(table);

            free(fileName);

        } else if (cfg_tokenStartsWith(token->head, RUN_KW)) {

            bool printTable = False;

            //Check for print flag.
            cfg_deleteToken(token);
            token = (Token*) malloc( sizeof(Token) );
            char* ptrToStatements = cfg_getNextToken(ptr, token, "", "+");
            printTable = cfg_isTokenKeyword(token->head, PRINT_TABLE_KW);

            if (!printTable)
                ptrToStatements = ptr;
            
            cfg_deleteToken(token);
            token = (Token*) malloc( sizeof(Token) );
            cfg_getNextToken(ptrToStatements, token, "", "");

            char* pBuffer = cfg_tokenToCString( token->head );  
            cfg_interpretStatements(pBuffer, symbolTable);

            if (printTable)
                cfg_printSymbolTable(symbolTable);
        }

        cfg_deleteToken(token);
    }

    free(symbolTable);

    return 0;
}