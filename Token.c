#include "Token.h"

#include <stdlib.h>

bool cfg_isTokenKeyword(Character* pHead, char* pKeyword) {

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