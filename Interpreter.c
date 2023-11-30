#include "Interpreter.h"
#include "Token.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

//Grammar Keywords --------------

const char* If_KW = "if";
const char* While_KW = "while";
const char* Else_KW = "else";

const char* True_KW = "true";
const char* False_KW = "false";

const char* ParenL_KW = "(";
const char* ParenR_KW = ")";
const char* SquigL_KW = "{";
const char* SquigR_KW = "}";

const char* Assign_KW = "=";
const char* Equals_KW = "==";
const char* LessTh_KW = "<";
const char* MoreTh_KW = ">";
const char* Inc_KW = "++";
const char* Dec_KW = "--";
const char* FloatPnt_KW = ".";

const char* Power_KW = "^";
const char* Plus_KW = "+";
const char* Minus_KW = "-";
const char* Mult_KW = "*";
const char* Divide_KW = "/";

const char* Print_KW = "print";

//-------------------------------

/*
    Returns whether or not token can be considered a symbol
    (i.e. if it contains letters).
    Returns true if it can be, false if it cannot be.
*/
bool cfg_isTokenSymbol(
    Character* pHead //Pointer to head of token.
    ) {

    while (pHead != NULL) {
        char data = pHead->data;
        //If the token has a...
        if (
            (data >= 65 && data <= 90) || //Uppercase letter, or
            (data >= 97 && data <= 122) //a Lowercase letter...
        ) {
            return True; //It must be a symbol. Symbols must contain at least one letter.
        }
        pHead = pHead->link;
    }
    return False;
}

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
    Creates a new symbol table entry for an uninitialized 
    symbol.
*/
void cfg_createSymbolTableEntry(
    Character* pHead,   //Head of token representing the symbol.
    SymbolTable* pTable //Pointer to symbol table.
    ) {

    pTable->symbolTokens[pTable->length] = pHead;
    pTable->types[pTable->length] = Uninitialized;
    pTable->length++;
    pTable->symbolTokens[pTable->length] = NULL;
    printf("Symbol Created!\n");
}

/*
    Checks if a symbol has been defined in the symbol table.
    Returns true if symbol is defined, false otherwise.
*/
bool cfg_isSymbolPresentInTable(
    Character* pHead,   //Head of token representing the symbol.
    SymbolTable* pTable //Pointer to symbol table.
    ) {

    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return True;
    }

    return False;
}

/*
    Returns the value of a symbol in the table.
    Returns the value of the symbol, or a value
        representing 0 if the symbol doesn't exist.
*/
Value cfg_getSymbolValue(
    Character* pHead,   //Head of token representing the symbol.
    SymbolTable* pTable //Pointer to symbol table.
    ) {

    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return pTable->values[i];
    }

    Value value;
    value.integer = 0;
    return value;
}

/*
    Returns the type of a symbol in the table.
    Returns the type of the symbol, or a type
        representing 'Integer' if the symbol
        doesn't exist.
*/
Type cfg_getSymbolType(
    Character* pHead,   //Head of token representing the symbol.
    SymbolTable* pTable //Pointer to symbol table.
    ) {

    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i]))
            return pTable->types[i];
    }

    return Integer;
}

/*
    Set the value and type of a symbol in the table
    (if it exists- does nothing otherwise).
*/
void cfg_setSymbolValue(
    Character* pHead,    //Head of token representing the symbol.
    SymbolTable* pTable, //Pointer to symbol table. 
    Type pType,          //Type we're assigning to the symbol.
    Value pVal           //Value we're assigning to the symbol.
    ) {

    for (int i = 0; i < pTable->length; i++) {
        if (cfg_tokensMatch(pHead, pTable->symbolTokens[i])) {
            pTable->types[i] = pType;
            pTable->values[i] = pVal;
            break;
        } 
    }
}

/*
    Prints information from the symbol table,
    including symbol names, and their corresponding
    types and values.
*/
void cfg_printSymbolTable(
    SymbolTable* pTable //Pointer to symbol table.
    ) {

    char* Type_Integer = "Integer";

    char* Type_Boolean = "Boolean";
    char* Bool_True_String = "True";
    char* Bool_False_String = "False";

    char* Type_Float = "Float";
    char* Type_Uninitialized = "Uninitialized";

    char* typeString;
    long long intValue;
    char* boolValue;
    double floatValue;

    printf("\n------ Symbol Table ------\n");
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
                printf(", Type: %s, Value: %"PRId64"\n", typeString, intValue);
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

/*
    Given a token, return either a float or integer value
    that the token represents.
    Returns a value containing either an integer or a float.
*/
Value cfg_parseFloatOrInteger(
    Character* pHead, //Head of token.
    Type* pType       //Pointer to type of value.
    ) {

    Value value;
    bool isNegative = cfg_tokenContainsKeyword(pHead, "-");

    //If negative, skip the sign.
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
        double upper = 0; double lower = 0;
        while (pHead->data != '.') {
            upper += ((int)(pHead->data - '0')) * pow(10.f, pwrUpper);
            pwrUpper--;
            pHead = pHead->link;
        }
        pHead = pHead->link;
        while (pHead != NULL) {
            lower += ((int)(pHead->data - '0')) / pow(10.f, pwrLower);
            pwrLower++;
            pHead = pHead->link;
        }

        value.floating_point = upper + lower;

        if (isNegative)
            value.floating_point *= -1;

    } else {
        (*pType) = Integer;

        long long val = 0; int pwr = -1;
        Character* nextChar = pHead;
        while (nextChar != NULL) { //Get power of ten for upper part of float.
            pwr++;
            nextChar = nextChar->link;
        }

        while (pHead != NULL) {
            val += ((long long)(pHead->data - '0')) * ((long long)pow(10.f, pwr));
            pwr--;
            pHead = pHead->link;
        }

        value.integer = val;

        if (isNegative)
            value.integer *= -1;
    }

    return value;

}

/*
    Given the current pointer to a char buffer representing the current file,
    assign a token so that it represents all of the characters not ignored, until 
    a character is encountered that is specified as 'terminating.'

    Returns a pointer to the buffer right after the terminating character.
*/
char* cfg_getNextToken(
    char* pBuffer,     //Pointer to current position in buffer.
    Token* pToken,     //Pointer to token we want to assign.
    char* pIgnored,    //C string representing characters to ignore.
    char* pTerminating //C string representing 'terminating' characters.
    ) {

    char nextChar = *pBuffer;
    pToken->head = (Character*) malloc(sizeof(Character));
    Character* nextCharacter = pToken->head;
    Character* head = nextCharacter;
    nextCharacter->link = NULL;

    char* ptrToTerminatingChar;

    while (True) {
        for (ptrToTerminatingChar = pTerminating; *ptrToTerminatingChar != '\0'; ptrToTerminatingChar++) {
            if (nextChar == *ptrToTerminatingChar || nextChar == '\0')
                goto outOfLoop;
        }

        bool ignore = False;
        for (char* ptr = pIgnored; *ptr != '\0'; ptr++) {
            if (nextChar == *ptr) {
                ignore = True;
                break;
            }
        }

        //Whitespace is always ignored.
        if (
            nextChar == ' ' ||
            nextChar == '\n' ||
            nextChar == '\r' ||
            nextChar == '\t' ||
            ignore
        ) {
            pBuffer++;
            nextChar = *pBuffer;
            continue;
        }

        nextCharacter->data = nextChar;
        nextCharacter->link = (Character*) malloc(sizeof(Character));
        head = nextCharacter;
        nextCharacter = nextCharacter->link;
        nextCharacter->link = NULL;
        pBuffer++;
        nextChar = *pBuffer;
    }

    outOfLoop:

    free(head->link);
    head->link = NULL;

    pToken->terminatingChar = nextChar;

    pBuffer++;

    return pBuffer;
}

/*
    Given a pointer to a character, deletes it and all
    of it's children.
*/
void cfg_deleteCharacters(
    Character* pCharacter //Pointer to character.
    ) {

    if (pCharacter->link != NULL)
        cfg_deleteCharacters(pCharacter->link);
    free(pCharacter);
}

/*
    Deletes a token (frees it's memory).
    Handles deleting it's character pointers as well.
*/
void cfg_deleteToken(
    Token* pToken //Pointer to token.
    ) {

    cfg_deleteCharacters(pToken->head);
    free(pToken);
}

/*
    Add two values, according to their types.
    Returns the sum of the two values.
*/
Value cfg_addValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB, //Second value.
    Type* pType  //Pointer to type of sum.
    ) {

    Value sum;

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }
    
    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    *pType = Integer;
                    sum.integer = pValA.integer + pValB.integer;
                    break;
                case Float:
                    *pType = Float;
                    sum.floating_point = ((double) pValA.integer) + pValB.floating_point;
                    break;
            }
            break;
        case Float:
            switch (pTypeB) {
                case Integer:
                    *pType = Float;
                    sum.floating_point = pValA.floating_point + ((double) pValB.integer);
                    break;
                case Float:
                    *pType = Float;
                    sum.floating_point = pValA.floating_point + pValB.floating_point;
                    break;
            }
            break;
    }

    return sum;
}

/*
    Subtract two values, according to their types.
    Returns the difference of the two values.
*/
Value cfg_subtractValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB, //Second value.
    Type* pType  //Pointer to type of difference.
    ) {

    Value diff;

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }
    
    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    *pType = Integer;
                    diff.integer = pValA.integer - pValB.integer;
                    break;
                case Float:
                    *pType = Float;
                    diff.floating_point = ((double) pValA.integer) - pValB.floating_point;
                    break;
            }
            break;
        case Float:
            switch (pTypeB) {
                case Integer:
                    *pType = Float;
                    diff.floating_point = pValA.floating_point - ((double) pValB.integer);
                    break;
                case Float:
                    *pType = Float;
                    diff.floating_point = pValA.floating_point - pValB.floating_point;
                    break;
            }
            break;
    }

    return diff;
}

/*
    Multiply two values, according to their types.
    Returns the product of the two values.
*/
Value cfg_multValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB, //Second value.
    Type* pType  //Pointer to type of product.
    ){

    Value product;

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }
    
    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    *pType = Integer;
                    product.integer = pValA.integer * pValB.integer;
                    break;
                case Float:
                    *pType = Float;
                    product.floating_point = ((double) pValA.integer) * pValB.floating_point;
                    break;
            }
            break;
        case Float:
            switch (pTypeB) {
                case Integer:
                    *pType = Float;
                    product.floating_point = pValA.floating_point * ((double) pValB.integer);
                    break;
                case Float:
                    *pType = Float;
                    product.floating_point = pValA.floating_point * pValB.floating_point;
                    break;
            }
            break;
    }

    return product;
}

/*
    Get a value to the power of another value.
    Returns the first value to the power of the other.
*/
Value cfg_powValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB, //Second value.
    Type* pType  //Pointer to type of product.
    ){

    Value product;

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }
    
    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    *pType = Integer;
                    product.integer = (int) pow(pValA.integer, pValB.integer);
                    break;
                case Float:
                    *pType = Float;
                    product.floating_point = pow(pValA.integer, pValB.floating_point);
                    break;
            }
            break;
        case Float:
            switch (pTypeB) {
                case Integer:
                    *pType = Float;
                    product.floating_point = pow(pValA.floating_point, pValB.integer);
                    break;
                case Float:
                    *pType = Float;
                    product.floating_point = pow(pValA.floating_point, pValB.floating_point);
                    break;
            }
            break;
    }

    return product;

}

/*
    Returns the value of the first value '&&' the other.
    Returns the first value 'and-ed' with the other one.
*/
bool cfg_andValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB  //Second value.
    ){

    if (pTypeA == Integer) {
        pTypeA = Boolean;
        if (pValA.integer >= 0)
            pValA.boolean = True;
        else
            pValA.integer = False;
    } else if (pTypeA == Float) {
        pTypeA = Boolean;
        if (pValA.floating_point >= 0)
            pValA.boolean = True;
        else
            pValA.boolean = False;
    }

    if (pTypeB == Integer) {
        pTypeB = Boolean;
        if (pValB.integer >= 0)
            pValB.boolean = True;
        else
            pValB.integer = False;
    } else if (pTypeB == Float) {
        pTypeB = Boolean;
        if (pValB.floating_point >= 0)
            pValB.boolean = True;
        else
            pValB.boolean = False;
    }
    
    return pValA.boolean && pValB.boolean;

}

/*
    Returns the value of the first value '||' the other.
    Returns the first value 'or-ed' with the other one.
*/
bool cfg_orValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB  //Second value.
    ){

    if (pTypeA == Integer) {
        pTypeA = Boolean;
        if (pValA.integer >= 0)
            pValA.boolean = True;
        else
            pValA.integer = False;
    } else if (pTypeA == Float) {
        pTypeA = Boolean;
        if (pValA.floating_point >= 0)
            pValA.boolean = True;
        else
            pValA.boolean = False;
    }

    if (pTypeB == Integer) {
        pTypeB = Boolean;
        if (pValB.integer >= 0)
            pValB.boolean = True;
        else
            pValB.integer = False;
    } else if (pTypeB == Float) {
        pTypeB = Boolean;
        if (pValB.floating_point >= 0)
            pValB.boolean = True;
        else
            pValB.boolean = False;
    }

    return pValA.boolean || pValB.boolean;

}

/*
    Returns the value of the first value '==' the other.
    Returns true if the values match, false if not.
*/
bool cfg_matchValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB  //Second value.
    ){

    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    return pValA.integer == pValB.integer;
                case Float:
                    //Round floating point value to nearest integer.
                    long long intB = pValB.floating_point >= 0 
                        ? (long long)pValB.floating_point + 0.5 
                        : (long long)pValB.floating_point - 0.5;
                    return pValA.integer == intB;
                case Boolean:
                    bool boolA = pValA.integer >= 0;
                    return boolA == pValB.boolean;
            }
        case Float:
            switch (pTypeB) {
                case Integer:
                    long long intA = pValA.floating_point >= 0 
                        ? (long long)pValA.floating_point + 0.5 
                        : (long long)pValA.floating_point - 0.5;
                    return intA == pValB.integer;
                case Float:
                    return fabs(pValA.floating_point - pValB.floating_point) <= FLOAT_COMP_EPSILON;
                case Boolean:
                    bool boolA = pValA.floating_point > 0;
                    return boolA == pValB.boolean;
            }
        case Boolean:
            bool boolB;
            switch (pTypeB) {
                case Integer:
                    boolB = pValB.integer > 0;
                    return pValA.boolean == boolB;
                case Float:
                    boolB = pValB.floating_point > 0;
                    return pValA.boolean == boolB;
                case Boolean:
                    return pValA.boolean == pValB.boolean;
            }
    }

    return False;
}

/*
    Returns the value of the first value '<' the other.
    Returns true if the first value is less than the other,
        false otherwise.
*/
bool cfg_lessThanValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB  //Second value.
    ){

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }

    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    return pValA.integer < pValB.integer;
                case Float:
                    return pValA.integer < pValB.floating_point;
            }
        case Float:
            switch (pTypeB) {
                case Integer:
                    return pValA.floating_point < pValB.integer;
                case Float:
                    return pValA.floating_point < pValB.floating_point;
            }
    }

    return False;
}

/*
    Returns the value of the first value '>' the other.
    Returns true if the first value is more than the other,
        false otherwise.
*/
bool cfg_moreThanValues(
    Type pTypeA, //Type of first value.
    Type pTypeB, //Type of second value.
    Value pValA, //First value.
    Value pValB  //Second value.
    ){

    if (pTypeA == Boolean) {
        pTypeA = Integer;
        if (pValA.boolean)
            pValA.integer = 1;
        else
            pValA.integer = 0;
    }

    if (pTypeB == Boolean) {
        pTypeB = Integer;
        if (pValB.boolean)
            pValB.integer = 1;
        else
            pValB.integer = 0;
    }

    switch (pTypeA) {
        case Integer:
            switch (pTypeB) {
                case Integer:
                    return pValA.integer > pValB.integer;
                case Float:
                    return pValA.integer > pValB.floating_point;
            }
        case Float:
            switch (pTypeB) {
                case Integer:
                    return pValA.floating_point > pValB.integer;
                case Float:
                    return pValA.floating_point > pValB.floating_point;
            }
    }

    return False;
}

/*
    Given a pointer to the start of an expression, return the value
    and type that the expression represents.

    If a math type operator is used, any booleans are converted to integers.
    If a logical operator is used, integers and floating pointers are converted
        to booleans, that are 'true' if more than zero, and 'false' if less than
        zero.
    If a symbol is used in an expression without already being defined, it will 
        represent a value of type 'Uninitialized' (see symbol table functions above).
    Any 'Uninitalized' values used in expressions are converted into integers with
        a value of 0.
*/
Value cfg_parseExpression(
    char* pBuffer,         //Pointer to start of expression the buffer.
    char* pEndOfExpr,      //Pointer to end of expression in the buffer. 
    Type* pType,           //Pointer to type of value returned.
    char pTerminatingChar, //Type of character to stop at when parsing token for value.
    SymbolTable* pTable    //Pointer to symbol table.
    ) {

    Value value;
    
    /* Find highest precedence operator.

       By highest precedence, I mean mathematically, and 
       the operator surrounded by the least number of parenthesis.
       When factoring in logical operators, I followed the same precedence as C,
       in relation to the math operators.
       If expression contains nested parenthesis expressions, the
       most nested expression is evaluated last, but since we will be recursing
       back up, it will be the first expression evaluated. */
    bool operatorFound = False;
    int lowestParenthesisCount = 2147483647; //Maximum value of integer.
    int currParenthesisCount = 0;
    char currOperator = '~';
    char* ptrToOperator;

    /* Do not want to mistake an expression like
       'Operator Value EndOfExpr' for an operator expression,
       and not the value expression it is. */
    if (
        *pBuffer == '+' ||
        *pBuffer == '-' ||
        *pBuffer == '*' ||
        *pBuffer == '/' ||
        *pBuffer == '^' ||
        *pBuffer == '<' ||
        *pBuffer == '>' 
        )
        pBuffer++;

    //Accounting for double width operators...
    else if (
        *pBuffer == '&' ||
        *pBuffer == '|' ||
        *pBuffer == '='
        )
        pBuffer += 2;
    
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
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '=' &&
                    currOperator != '<' && currOperator != '>'
                    ) {
                    currOperator = '+';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '-':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '=' &&
                    currOperator != '<' && currOperator != '>'
                    ) {
                    currOperator = '-';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '*':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '=' &&
                    currOperator != '<' && currOperator != '>' && 
                    currOperator != '+' && currOperator != '-'
                    ) {
                    currOperator = '*';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '/':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '=' &&
                    currOperator != '<' && currOperator != '>' &&
                    currOperator != '+' && currOperator != '-'
                    ) {
                    currOperator = '/';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '^':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '=' &&
                    currOperator != '<' && currOperator != '>' &&
                    currOperator != '+' && currOperator != '-' &&
                    currOperator != '*' && currOperator != '/'
                    ) {
                    currOperator = '^';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '>':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '='
                    ) {
                    currOperator = '>';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '<':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&' &&
                    currOperator != '='
                    ) {
                    currOperator = '<';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                break;
            case '=':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|' &&
                    currOperator != '&'
                    ) {
                    currOperator = '=';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                ptr++; //Accounting for double width operators...
                break;
            case '&':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount && 
                    currOperator != '|'
                    ) {
                    currOperator = '&';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                ptr++;
                break;
            case '|':
                operatorFound = True;
                if (currParenthesisCount <= lowestParenthesisCount) {
                    currOperator = '|';
                    lowestParenthesisCount = currParenthesisCount;
                    ptrToOperator = ptr;
                }
                ptr++;
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
        char symbols[3] = {')', pTerminatingChar, '\0'};
        cfg_getNextToken(pBuffer, token, "(", symbols);

        //Expression -> 'true'
        if (cfg_isTokenKeyword(token->head, True_KW)) {
            (*pType) = Boolean;
            value.boolean = True;
        //Expression -> 'false'
        } else if (cfg_isTokenKeyword(token->head, False_KW)) {
            (*pType) = Boolean;
            value.boolean = False;
        //Expression -> Symbol
        } else if (cfg_isTokenSymbol(token->head)) {
            (*pType) = cfg_getSymbolType(token->head, pTable);
            value = cfg_getSymbolValue(token->head, pTable);

            if (*pType == Uninitialized) {
                *pType = Integer;
            }
        //Expression -> Float | Integer
        } else {
            value = cfg_parseFloatOrInteger(token->head, pType);
        }

        cfg_deleteToken(token);

    //Expression -> Expression Operator Expression 
    } else {

        Value valA, valB;
        Type typeA, typeB;
        
        /*
            valA is the expression to the left of the operator, and
            valB is the expression to the right of the operator.

            Expressions are parsed with left precedence, so valA
            is parsed first.
        */

        valA = cfg_parseExpression(pBuffer, ptrToOperator, &typeA, currOperator, pTable);
        valB = cfg_parseExpression(ptrToOperator, pEndOfExpr, &typeB, pTerminatingChar, pTable);

        switch (currOperator) {
            case '+':
                value = cfg_addValues(typeA, typeB, valA, valB, pType);
                break;
            case '-':
                value = cfg_subtractValues(typeA, typeB, valA, valB, pType);
                break;
            case '*':
                value = cfg_multValues(typeA, typeB, valA, valB, pType);
                break;
            case '^':
                value = cfg_powValues(typeA, typeB, valA, valB, pType);
                break;
            case '<':
                *pType = Boolean;
                value.boolean = cfg_lessThanValues(typeA, typeB, valA, valB);
                break;
            case '>':
                *pType = Boolean;
                value.boolean = cfg_moreThanValues(typeA, typeB, valA, valB);
                break;
            case '=':
                *pType = Boolean;
                value.boolean = cfg_matchValues(typeA, typeB, valA, valB);
                break;
            case '&':
                *pType = Boolean;
                value.boolean = cfg_andValues(typeA, typeB, valA, valB);
                break;
            case '|':
                *pType = Boolean;
                value.boolean = cfg_orValues(typeA, typeB, valA, valB);
                break;
            default:
                printf("Operator Unrecognized (Returning integer 0).\n");
                *pType = Integer;
                value.integer = 0;
                break;
        }

    }

    return value;

}

/*
    Used to help determine what to do if 
    we reach the end of a block
    (i.e. a token terminates with '}').
*/
typedef enum {
    While, If, IfElse, Else, None
} ControlBlockType;

/*
    Main loop for interpreting a file.
*/
void cfg_loop(
    char* pBuffer, 
    SymbolTable* pTable
    ) {

    ControlBlockType controlBlockStack[MAX_CONTROL_BLOCKS];
    int controlBlockStackIndex = 0;

    controlBlockStack[0] = None;

    //Pointers to use when re-evaluating while conditionals.
    char* conditionExprPtrs[MAX_NESTED_WHILE_LOOPS];
    //Pointers to end of while conditional expressions.
    char* endsOfConditionalExp[MAX_NESTED_WHILE_LOOPS];
    //Pointers to while block bodies.
    char* ptrsToWhileBody[MAX_NESTED_WHILE_LOOPS];
    //Number of nested while loops.
    int whileLoopCount = 0;

    while (True) {

        Token* token = (Token*) malloc(sizeof(Token));
        pBuffer = cfg_getNextToken(pBuffer, token, "{", "(}=;");

        /*
            String -> ' " ' Any characters ' " '
            Statement -> PrintLn Statement
            Println Statement -> 'print (' String ');'
        */
        if (cfg_isTokenKeyword(token->head, Print_KW)) {

            while (*pBuffer != '\"')
                pBuffer++;
            pBuffer++;

            for (; *pBuffer != '\"'; pBuffer++) {

                char nextChar = *pBuffer;

                if (nextChar == '\n' ||
                    nextChar == '\r' ||
                    nextChar == '\t')
                    continue;
                
                if (nextChar == '\\') {
                    nextChar = *(pBuffer+1);
                    switch (nextChar) {
                        case '\"':
                            printf("\"");
                            pBuffer++;
                            break;
                        case 'n':
                            printf("\n");
                            pBuffer++;
                            break;
                        case 'r':
                            printf("\r");
                            pBuffer++;
                            break;
                        case 't':
                            printf("\t");
                            pBuffer++;
                            break;
                    }
                } else {
                    printf("%c", nextChar);
                }

            }

            while (*pBuffer != ';')
                pBuffer++;
            pBuffer++;

        /*
            Statement -> While Block
            While Block -> 'while (' Expression ') {' Statement '}'
        */
        } else if (cfg_isTokenKeyword(token->head, While_KW)) {

            conditionExprPtrs[whileLoopCount] = pBuffer;

            //Get pointer to the end of the expression.
            int parenthesisCount = 1;
            char* endOfExpr;
            for (endOfExpr = pBuffer; parenthesisCount > 0; endOfExpr++) {
                if (*endOfExpr == '(')
                    parenthesisCount++;
                else if (*endOfExpr == ')')
                    parenthesisCount--;
            }

            Type type;
            Value value = cfg_parseExpression(conditionExprPtrs[whileLoopCount], endOfExpr, &type, ')', pTable);
            
            endsOfConditionalExp[whileLoopCount] = endOfExpr;
            pBuffer = endOfExpr;
            pBuffer++;

            ptrsToWhileBody[whileLoopCount] = pBuffer;

            if (type != Boolean)
                value.boolean = type == Integer ? value.integer > 0 : value.floating_point > 0;

            if (value.boolean) {

                whileLoopCount++;

                controlBlockStackIndex++;
                controlBlockStack[controlBlockStackIndex] = While;

            //Skip while loop is conditional is false in the first place.
            } else {

                while (*pBuffer != '{')
                    pBuffer++;
                pBuffer++;
   
                int braceCount = 1;
                for (; braceCount > 0; pBuffer++) {
                    if (*pBuffer == '{')
                        braceCount++;
                    else if (*pBuffer == '}')
                        braceCount--;
                }

            }
            
            cfg_deleteToken(token);

        /*
            Statement -> If Block
            If Block -> 'if (' Expression ') {' Statement '}' |
                        'if (' Expression ') {' Statement '} else {' Statement '}'
        */
        } else if (cfg_isTokenKeyword(token->head, If_KW)) {

            //Get pointer to the end of the expression.
            int parenthesisCount = 1;
            char* endOfExpr;
            for (endOfExpr = pBuffer; parenthesisCount > 0; endOfExpr++) {
                if (*endOfExpr == '(')
                    parenthesisCount++;
                else if (*endOfExpr == ')')
                    parenthesisCount--;
            }

            Type type;
            Value value = cfg_parseExpression(pBuffer, endOfExpr, &type, ')', pTable);
           
            pBuffer = endOfExpr;
            while (*pBuffer != '{')
                pBuffer++;
            pBuffer++;

            if (type != Boolean)
                value.boolean = type == Integer ? value.integer > 0 : value.floating_point > 0;

            //Check and see if there's an else block attached to the if block.
            int braceCount = 1;
            char* endIfPtr;
            for (endIfPtr = pBuffer; braceCount > 0; endIfPtr++) {
                if (*endIfPtr == '{')
                    braceCount++;
                else if (*endIfPtr == '}')
                    braceCount--;
            }

            cfg_deleteToken(token);
            token = (Token*) malloc(sizeof(Token));

            char* elsePtr = cfg_getNextToken(endIfPtr, token, "", "{");

            bool hasElseBlock = cfg_isTokenKeyword(token->head, Else_KW);

            if (value.boolean && hasElseBlock) {
                controlBlockStackIndex++;
                controlBlockStack[controlBlockStackIndex] = IfElse;
            } else if (value.boolean) {
                controlBlockStackIndex++;
                controlBlockStack[controlBlockStackIndex] = If;
            } else if (hasElseBlock) {
                pBuffer = elsePtr;
                controlBlockStackIndex++;
                controlBlockStack[controlBlockStackIndex] = Else;
            } else {
                pBuffer = endIfPtr;
            }

            cfg_deleteToken(token);

        } else {

            /*
                Statement -> Definition
                Definition -> Symbol ';'
            */
            if (token->terminatingChar == ';') { 
                cfg_createSymbolTableEntry(token->head, pTable);

            /*
                Statement -> Assignment
                Assignment -> Symbol '=' Expression ';'
            */
            } else if (token->terminatingChar == '=') {

                //Track if symbol is already in table so we know or not to delete it at the end.
                bool symbolInTable = cfg_isSymbolPresentInTable(token->head, pTable);
                if (!symbolInTable)
                    cfg_createSymbolTableEntry(token->head, pTable);

                char* endOfExpr;
                for (endOfExpr = pBuffer; *endOfExpr != ';'; endOfExpr++);
                
                Type type;
                Value value = cfg_parseExpression(pBuffer, endOfExpr, &type, ';', pTable);
                
                pBuffer = endOfExpr;
                pBuffer++;

                cfg_setSymbolValue(token->head, pTable, type, value);

                if (symbolInTable)
                    cfg_deleteToken(token);


            } else if (token->terminatingChar == '}') {

                cfg_deleteToken(token);

                switch (controlBlockStack[controlBlockStackIndex]) {
                    case While:
                        //Revaluate conditional expression.
                        Type type;
                        Value value = cfg_parseExpression(
                            conditionExprPtrs[whileLoopCount-1], 
                            endsOfConditionalExp[whileLoopCount-1], 
                            &type, ')', pTable);

                        if (type != Boolean)
                            value.boolean = type == Integer ? value.integer > 0 : value.floating_point > 0;

                        if (value.boolean)
                            pBuffer = ptrsToWhileBody[whileLoopCount-1];
                        else {
                            whileLoopCount--;
                            controlBlockStackIndex--;
                        }
                        break;
                    case IfElse:
                        token = (Token*) malloc(sizeof(Token));
                        pBuffer = cfg_getNextToken(pBuffer, token, "", "{");

                        int braceCount = 1;
                        for (; braceCount > 0; pBuffer++) {
                            if (*pBuffer == '{')
                                braceCount++;
                            else if (*pBuffer == '}')
                                braceCount--;
                        }

                        cfg_deleteToken(token);

                        controlBlockStackIndex--;
                        break;
                    case If:
                        controlBlockStackIndex--;
                        break;
                    case Else:
                        controlBlockStackIndex--;
                        break;
                    case None:
                        break;
                }                  

            //Statement -> (None)
            } else if (token->terminatingChar == '\0') {
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