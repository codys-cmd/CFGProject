#include <stdlib.h>

//Boolean definitions.
typedef int bool;
const bool False = 0;
const bool True = 1;

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
//-------------------------------

typedef struct Character Character;

struct Character {
    char data;
    Character* link;
};

typedef struct {
    char terminatingSymbol;
    Character* head;
} Token;

bool cfg_isTokenKeyword(Character* pHead, const char* pKeyword) {

    while ((*pKeyword) != '\0') {
        //If Token is Shorter than keyword.
        if (pHead == NULL) {
            return False;
        }
        //If Token has a charater that doesn't match keyword.
        if (pHead->data != (*pKeyword)) {
            return False;
        }
        pHead = pHead->link;
        pKeyword++;
    }

    //If keyword is shorter than token.
    return pHead == NULL;

}

bool cfg_tokenContainsKeyword(Character* pHead, char* pKW) {

    char* curKWChar = pKW;

    while (pHead != NULL) {

        if (*curKWChar == '\0')
            return True;

        if (pHead->data == (*curKWChar)) {
            curKWChar++;
        } else {
            curKWChar = pKW;
        }

        pHead = pHead->link;

    }

    return curKWChar == NULL;

}

bool cfg_isTokenSymbol(Character* pHead) {
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

bool cfg_tokensMatch(Character* pHeadA, Character* pHeadB) {

    while (pHeadB != NULL) {

        //If Token is Shorter than the other.
        if (pHeadA == NULL) {
            return False;
        }

        //If Token has a charater that doesn't match the other.
        if (pHeadA->data != pHeadB->data) {
            return False;
        }

        pHeadA = pHeadA->link;
        pHeadB = pHeadB->link;
    }

    //If the other token is shorter.
    return pHeadA == NULL;

}