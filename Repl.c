#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include "Interpreter.h"

#define INPUT_BUFFER_LENGTH 256
#define TO_STRING(x) Stringify(x)
    #define Stringify(x) #x
char inputBuffer[INPUT_BUFFER_LENGTH];

//REPL Keywords
char* HELP = "-help";
char* QUIT = "-q";
char* SYMBOLS = "-symbols";
char* RUNFILE = "-runfile";
char* USE_REPL_TABLE = "s";
char* PRINT_TABLE = "p";
char* RUN = "-run";
char* DEL_SYMBOLS = "-del_symbols";

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
        if (cfg_isTokenKeyword(token->head, QUIT)) {
            printf("Quitting Repl!\n");
            isRunning = False;
        
        //"-help" - Prints information about commands you can run.
        } else if (cfg_isTokenKeyword(token->head, HELP)) {
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
        } else if (cfg_isTokenKeyword(token->head, SYMBOLS)) {
            cfg_printSymbolTable(symbolTable);

        //"-runfile FLAGS +FILE_NAME" - Runs a file.
        } else if (cfg_tokenStartsWith(token->head, RUNFILE)) {

            bool useReplTable = False;
            bool printTable = False;

            //Check for flags.
            while (token->terminatingChar == '+') {
                cfg_deleteToken(token);
                token = (Token*) malloc( sizeof(Token) );
                ptr = cfg_getNextToken(ptr, token, "", "+");
                if (!useReplTable) {
                    useReplTable = cfg_isTokenKeyword(token->head, USE_REPL_TABLE);
                } 
                if (!printTable) {
                    printTable = cfg_isTokenKeyword(token->head, PRINT_TABLE);
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
                free(table);

            free(fileName);

        } else if (cfg_tokenStartsWith(token->head, RUN)) {

            bool printTable = False;

            //Check for print flag.
            cfg_deleteToken(token);
            token = (Token*) malloc( sizeof(Token) );
            char* ptrToStatements = cfg_getNextToken(ptr, token, "", "+");
            printTable = cfg_isTokenKeyword(token->head, PRINT_TABLE);

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