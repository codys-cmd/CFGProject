#include <stdio.h>
#include <math.h>
#include "Token.h"

typedef union {
    int integer;
    bool boolean;
    float floating_point;
} Value;

typedef enum {
    Integer, Boolean, Float, Uninitialized
} Type;

//Maps symbols to values via their indicies
//Max Symbols = 128
typedef struct {
    
    int length;
    Character* symbolTokens [128];
    Type types [128];
    Value values [128];

} SymbolTable;

void cfg_createSymbolTableEntry(Character* pHead, SymbolTable* pTable) {
    pTable->symbolTokens[pTable->length] = pHead;
    pTable->types[pTable->length] = Uninitialized;
    pTable->length++;
    pTable->symbolTokens[pTable->length] = NULL;
    printf("Symbol Created!\n");
}

bool cfg_isSymbolPresentInTable(Character* pHead, SymbolTable* pTable) {
    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return True;
    }

    return False;
}

Value cfg_getSymbolValue(Character* pHead, SymbolTable* pTable) {
    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return pTable->values[i];
    }
}

Type cfg_getSymbolType(Character* pHead, SymbolTable* pTable) {
    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return pTable->types[i];
    }
}

void cfg_setSymbolValue(Character* pHead, SymbolTable* pTable, Type pType, Value pVal) {
    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i])) {
            pTable->types[i] = pType;
            pTable->values[i] = pVal;
            break;
        } 
    }
}

Value cfg_parseValue(Character* pHead, Type* pType) {
    Value value;
    bool isNegative = cfg_tokenContainsKeyword(pHead, "-");
    //If negative, skip that sign.
    if (isNegative)
        pHead = pHead->link;
    if (cfg_tokenContainsKeyword(pHead, FloatPnt_KW)) {

        (*pType) = Float;

        int pwrUpper = -1; int pwrLower = 1;
        Character* nextChar = pHead;
        while (nextChar->data != '.') { //Get power of ten for upper part of float.
            pwrUpper++;
            nextChar = nextChar->link;
        }
        nextChar = nextChar->link; //Skip floating point character.
        float upper = 0; float lower = 0;
        while (pHead->data != '.') {
            upper += ((int)(pHead->data - '0'))*powf(10.f, pwrUpper);
            pwrUpper--;
            pHead = pHead->link;
        }
        pHead = pHead->link;
        while (pHead != NULL) {
            lower += ((int)(pHead->data - '0'))/powf(10.f, pwrLower);
            pwrLower++;
            pHead = pHead->link;
        }

        value.floating_point = upper + lower;

        if (isNegative)
            value.floating_point *= -1;

    } else {
        (*pType) = Integer;

        int val = 0; int pwr = -1;
        Character* nextChar = pHead;
        while (nextChar != NULL) { //Get power of ten for upper part of float.
            pwr++;
            nextChar = nextChar->link;
        }

        while (pHead != NULL) {
            val += ((int)(pHead->data - '0'))*((int)powf(10.f, pwr));
            pwr--;
            pHead = pHead->link;
        }

        value.integer = val;

        if (isNegative)
            value.integer *= -1;
    }

    return value;

}

void cfg_printSymbolTable(SymbolTable* pTable) {

    char* Type_Integer = "Integer";

    char* Type_Boolean = "Boolean";
    char* Bool_True_String = "True";
    char* Bool_False_String = "False";

    char* Type_Float = "Float";
    char* Type_Uninitialized = "Uninitialized";

    char* typeString;
    int intValue;
    char* boolValue;
    float floatValue;

    printf("------ Symbol Table ------\n");
    for (int i = 0; i < pTable->length; i++) {
        printf("Symbol %d. Name: ", i);
        Character* head = pTable->symbolTokens[i];
        while (head != NULL) {
            printf("%c", head->data);
            head = head->link;
        }
        switch (pTable->types[i]) {
            case Integer:
                intValue = pTable->values[i].integer;
                typeString = Type_Integer;
                printf(", Type: %s, Value: %d\n", typeString, intValue);
                break;
            case Boolean:
                if (pTable->values[i].boolean)
                    boolValue = Bool_True_String;
                else
                    boolValue = Bool_False_String;
                typeString = Type_Boolean;
                printf(", Type: %s, Value: %s\n", typeString, boolValue);
                break;
            case Float:
                floatValue = pTable->values[i].floating_point;
                typeString = Type_Float;
                printf(", Type: %s, Value: %f\n", typeString, floatValue);
                break;
            case Uninitialized:
                typeString = Type_Uninitialized;
                printf(", Type: %s, Value: %s\n", typeString, "N/A");
                break;
        }
    }
}

char* cfg_getNextToken(char* pBuffer, Token* pToken, const char* pIgnored, const char* pSymbols) {

    char nextSymbol = *pBuffer;
    pToken->head = (Character*) malloc(sizeof(Character));
    Character* nextCharacter = pToken->head;
    Character* head = nextCharacter;
    nextCharacter->link = NULL;

    char* ptrToTerminatingSymbol;

    while (True) {
        for (ptrToTerminatingSymbol = pSymbols; *ptrToTerminatingSymbol != '\0'; ptrToTerminatingSymbol++) {
            if (nextSymbol == *ptrToTerminatingSymbol || nextSymbol == '\0')
                goto outOfLoop;
        }

        bool ignore = False;
        for (char* ptr = pIgnored; *ptr != '\0'; ptr++) {
            if (nextSymbol == *ptr) {
                ignore = True;
                break;
            }
        }

        //Whitespace is always ignored.
        if (
            nextSymbol == ' ' ||
            nextSymbol == '\n' ||
            nextSymbol == '\r' ||
            ignore
        ) {
            pBuffer++;
            nextSymbol = *pBuffer;
            continue;
        }

        nextCharacter->data = nextSymbol;
        nextCharacter->link = (Character*) malloc(sizeof(Character));
        head = nextCharacter;
        nextCharacter = nextCharacter->link;
        nextCharacter->link = NULL;
        pBuffer++;
        nextSymbol = *pBuffer;
    }

    outOfLoop:

    free(head->link);
    head->link = NULL;

    pToken->terminatingSymbol = nextSymbol;

    pBuffer++;

    return pBuffer;
}

void cfg_deleteCharacters(Character* pCharacter) {
    if (pCharacter->link != NULL)
        cfg_deleteCharacters(pCharacter->link);
    free(pCharacter);
}

void cfg_deleteToken(Token* pToken) {
    cfg_deleteCharacters(pToken->head);
    free(pToken);
}

Value cfg_addValues(Type pTypeA, Type pTypeB, Value pValA, Value pValB, Type* sumType) {
    Value sum;
    if (pTypeA == Integer) {
        if (pTypeB == Integer) {
            (*sumType) = Integer;
            sum.integer = pValA.integer + pValB.integer;
        } else if (pTypeB == Float) {
            (*sumType) = Float;
            sum.floating_point = ((float) pValA.integer) + pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*sumType) = Integer;
            sum.integer = pValA.integer + pValB.boolean;
        } else if (pTypeB == Uninitialized) {
            (*sumType) = Integer;
            sum.integer = pValA.integer;
        }
    } else if (pTypeA == Float) {
        if (pTypeB == Integer) {
            (*sumType) = Float;
            sum.integer = pValA.floating_point + ((float)pValB.integer);
        } else if (pTypeB == Float) {
            (*sumType) = Float;
            sum.floating_point = pValA.floating_point + pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*sumType) = Float;
            sum.floating_point = pValA.floating_point + ((float)pValB.boolean);
        } else if (pTypeB == Uninitialized) {
            (*sumType) = Float;
            sum.floating_point = pValA.floating_point;
        }
    } else if (pTypeA == Boolean) {
        if (pTypeB == Integer) {
            (*sumType) = Integer;
            sum.integer = pValA.boolean + pValB.integer;
        } else if (pTypeB == Float) {
            (*sumType) = Float;
            sum.floating_point = ((float)pValA.boolean) + pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*sumType) = Integer;
            sum.integer = pValA.integer + pValB.integer;
        } else if (pTypeB == Uninitialized) {
            (*sumType) = Integer;
            sum.integer = pValA.boolean;
        }
    } else if (pTypeA == Uninitialized) {
        if (pTypeB == Integer) {
            (*sumType) = Integer;
            sum.integer = pValB.integer;
        } else if (pTypeB == Float) {
            (*sumType) = Float;
            sum.floating_point = pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*sumType) = Integer;
            sum.integer = pValB.boolean;
        } else if (pTypeB == Uninitialized) {
            (*sumType) = Integer;
            sum.integer = 0;
        }
    }

    return sum;
}

Value cfg_subValues(Type pTypeA, Type pTypeB, Value pValA, Value pValB, Type* diffType) {
    Value diff;
    if (pTypeA == Integer) {
        if (pTypeB == Integer) {
            (*diffType) = Integer;
            diff.integer = pValA.integer - pValB.integer;
        } else if (pTypeB == Float) {
            (*diffType) = Float;
            diff.floating_point = ((float) pValA.integer) - pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*diffType) = Integer;
            diff.integer = pValA.integer - pValB.boolean;
        } else if (pTypeB == Uninitialized) {
            (*diffType) = Integer;
            diff.integer = pValA.integer;
        }
    } else if (pTypeA == Float) {
        if (pTypeB == Integer) {
            (*diffType) = Float;
            diff.integer = pValA.floating_point - ((float)pValB.integer);
        } else if (pTypeB == Float) {
            (*diffType) = Float;
            diff.floating_point = pValA.floating_point - pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*diffType) = Float;
            diff.floating_point = pValA.floating_point - ((float)pValB.boolean);
        } else if (pTypeB == Uninitialized) {
            (*diffType) = Float;
            diff.floating_point = pValA.floating_point;
        }
    } else if (pTypeA == Boolean) {
        if (pTypeB == Integer) {
            (*diffType) = Integer;
            diff.integer = pValA.boolean - pValB.integer;
        } else if (pTypeB == Float) {
            (*diffType) = Float;
            diff.floating_point = ((float)pValA.boolean) - pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*diffType) = Integer;
            diff.integer = pValA.integer - pValB.integer;
        } else if (pTypeB == Uninitialized) {
            (*diffType) = Integer;
            diff.integer = pValA.boolean;
        }
    } else if (pTypeA == Uninitialized) {
        if (pTypeB == Integer) {
            (*diffType) = Integer;
            diff.integer = -pValB.integer;
        } else if (pTypeB == Float) {
            (*diffType) = Float;
            diff.floating_point = -pValB.floating_point;
        } else if (pTypeB == Boolean) {
            (*diffType) = Integer;
            diff.integer = -pValB.boolean;
        } else if (pTypeB == Uninitialized) {
            (*diffType) = Integer;
            diff.integer = 0;
        }
    }

    return diff;
}

Value cfg_parseExpression(
    char* pBuffer, 
    char* pEndOfExpr, 
    Type* pType,
    char pTerminatingSymbol,
    SymbolTable* pTable) {

    Value value;
    
    /* Find highest precedence operator.
       By highest precedence, I mean mathematically, and 
       the operator surrounded by the least number of parenthesis.
       If expression contains nested parenthesis expressions, the
       most nested expression is evaluated last, but since we will be recursing
       back up, it will be the first expression evaluated. */
    bool operatorFound = False;
    int lowestParenthesisCount = 2147483647; //Maximum value of integer.
    int currParenthesisCount = 0;
    char currOperator = '~';
    char* ptrToOperator;

    //Do not want to mistake an 'Operator Value' for a real expression.
    if (*pBuffer == '+' ||
        *pBuffer == '-' ||
        *pBuffer == '*' ||
        *pBuffer == '/')
        pBuffer++;
    
    for (char* ptr = pBuffer; ptr != pEndOfExpr; ptr++) {
        char symbol = *ptr;
        switch (symbol) {
            case '(':
                currParenthesisCount++;
                break;
            case ')':
                currParenthesisCount--;
                break;
            case '+':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount) {
                    currOperator = '+';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '-':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount) {
                    currOperator = '-';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '*':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                /* Since we'll be recursing backward when actually evaluating,
                   '+' and '-' actually have a higher precedence than '*' and '/'. */
                    (currOperator != '+' || currOperator != '-')) {
                    currOperator = '*';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '/':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    (currOperator != '+' || currOperator != '-')) {
                    currOperator = '/';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
        }
    }

    /*Expression -> Value
      Value -> Symbol | Literal
      Symbol -> Some token with letters in it.
      Literal -> Float | Integer | 'false' | 'true'
    */
    if (!operatorFound) {

        Token* token = (Token*) malloc(sizeof(Token));
        char symbols[3] = {')', pTerminatingSymbol, '\0'};
        cfg_getNextToken(pBuffer, token, "(", symbols);

        if (cfg_isTokenKeyword(token->head, True_KW)) {
            (*pType) = Boolean;
            value.boolean = True;
        //Expression -> false
        } else if (cfg_isTokenKeyword(token->head, False_KW)) {
            (*pType) = Boolean;
            value.boolean = False;
        //Expression -> Symbol
        } else if (cfg_isTokenSymbol(token->head)) {
            (*pType) = cfg_getSymbolType(token->head, pTable);
            value = cfg_getSymbolValue(token->head, pTable);
        //Expression -> Float | Integer
        } else {
            value = cfg_parseValue(token->head, pType);
        }

        cfg_deleteToken(token);

    //valA Operator valB
    /* Expression -> Expression Operator Expression 
        >> Left Precedence -> valA is Evaulated first. <<
    */
    } else {

        Value valA, valB;
        Type typeA, typeB;

        valA = cfg_parseExpression(pBuffer, ptrToOperator, &typeA, currOperator, pTable);
        valB = cfg_parseExpression(ptrToOperator, pEndOfExpr, &typeB, pTerminatingSymbol, pTable);

        switch (currOperator) {
            case '+':
                value = cfg_addValues(typeA, typeB, valA, valB, pType);
                break;
            case '-':
                value = cfg_subValues(typeA, typeB, valA, valB, pType);
                break;
            default:
                printf("Operator Unimplemented (Yet!)\nReturning number 0.\n");
                *pType = Integer;
                value.integer = 0;
                break;
        }

    }

    return value;

}

void cfg_loop(char* pBuffer, SymbolTable* pTable) {

    typedef enum {
        While, If, Else
    } ControlBlockType;

    ControlBlockType controlBlockType;
    bool inControlBlock = False;
    char* pointerToConditionalExp;

    while (True) {

        Token* token = (Token*) malloc(sizeof(Token));
        pBuffer = cfg_getNextToken(pBuffer, token, "", "(=;");

        if (cfg_isTokenKeyword(token->head, While_KW)) { 
            //Parse While Block

            pointerToConditionalExp = pBuffer;

            int parenthesisCount = 1;
            char* endOfExpr;
            for (endOfExpr = pBuffer; parenthesisCount > 0; endOfExpr++) {
                if (*endOfExpr == '(')
                    parenthesisCount++;
                else if (*endOfExpr == ')')
                    parenthesisCount--;
            }

            Type type;
            Value value = cfg_parseExpression(pointerToConditionalExp, endOfExpr, &type, ')', pTable);
            
            pBuffer = endOfExpr;
            if (*pBuffer == ';' || *pBuffer == ')')
                pBuffer++;

            inControlBlock = value.boolean;

            if (inControlBlock)
                controlBlockType = While;

            cfg_deleteToken(token);
        } else if (cfg_isTokenKeyword(token->head, If_KW)) {
            //Parse If Block
            pointerToConditionalExp = pBuffer;

            int parenthesisCount = 1;
            char* endOfExpr;
            for (endOfExpr = pBuffer; parenthesisCount > 0; endOfExpr++) {
                if (*endOfExpr == '(')
                    parenthesisCount++;
                else if (*endOfExpr == ')')
                    parenthesisCount--;
            }

            Type type;
            Value value = cfg_parseExpression(pointerToConditionalExp, endOfExpr, &type, ')', pTable);
           
            pBuffer = endOfExpr;
            if (*pBuffer == ';' || *pBuffer == ')')
                pBuffer++;
            inControlBlock = value.boolean;
            
            if (inControlBlock)
                controlBlockType = If;

            cfg_deleteToken(token);
        } else {
            if (token->terminatingSymbol == ';') { 
                //Parse Definition
                cfg_createSymbolTableEntry(token->head, pTable);
            } else if (token->terminatingSymbol == '=') {

                printf("= Encountered!\n");

                //Track if symbol is already in table so we know or not to delete it at the end.
                bool symbolInTable = cfg_isSymbolPresentInTable(token->head, pTable);
                if (!symbolInTable)
                    cfg_createSymbolTableEntry(token->head, pTable);

                char* endOfExpr;
                for (endOfExpr = pBuffer;  *endOfExpr != ';'; endOfExpr++);
                
                Type type;
                Value value = cfg_parseExpression(pBuffer, endOfExpr, &type, ';', pTable);
                
                pBuffer = endOfExpr;
                if (*pBuffer == ';' || *pBuffer == ')')
                    pBuffer++;

                cfg_setSymbolValue(token->head, pTable, type, value);

                if (symbolInTable)
                    cfg_deleteToken(token);

            } else if (token->terminatingSymbol == '\0') {
                printf("EOF Encountered!\n");
                cfg_deleteToken(token);
                break;
            }
        }
    }
}

void cfg_start(const char* pFileName) {

    //Open file and get length.
    FILE* file = fopen(pFileName, "rb");
    fseek(file, 0, SEEK_END);
    int lengthOfFile = ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate buffer for string rep. of file.
    char* buffer = (char*) malloc(lengthOfFile + 1);
    fread(buffer, 1, lengthOfFile, file);
    fclose(file);
    buffer[lengthOfFile] = '\0';

    //Allocate and initalize Symbol Table.
    SymbolTable* table = (SymbolTable*) malloc(sizeof(SymbolTable));
    table->length = 0;
    table->symbolTokens[0] = NULL;

    //Start main loop.
    cfg_loop(buffer, table);

    //Free buffer
    free(buffer);

    //Print Table
    cfg_printSymbolTable(table);

    //Free Table
    free(table);
}