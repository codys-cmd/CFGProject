#include "Token.h"

#include <stdlib.h>

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

bool cfg_tokenContainsKeyword(Character* pHead, const char* pKW) {

    const char* curKWChar = pKW;

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

bool cfg_tokenStartsWith(Character* pHead, const char* pKeyword) {
    while (True) {
        if (*pKeyword == '\0')
            return True;

        if (pHead == NULL)
            return False;
        
        if (pHead->data == *pKeyword)
            pKeyword++;
        else
            return False;
        
        pHead = pHead->link;
    }
    return False;
}

Character* cfg_returnCharsAfterIndex(Character* pHead, int index) {
    while (index > 0) {
        if (pHead == NULL)
            return NULL;
        pHead = pHead->link;
        index--;
    }
    return pHead;
}

char* cfg_tokenToCString(Character* pHead) {
    int len = 1;
    Character* nextChar = pHead;
    while (nextChar != NULL) {
        len++;
        nextChar = nextChar->link;
    }
    char* str = (char*) malloc(len * sizeof(char));
    for (int i = 0; i < len-1; i++) {
        str[i] = pHead->data;
        pHead = pHead->link;
    }

    str[len-1] = '\0';
    return str;
}

char* cfg_getNextToken(char* pBuffer, Token* pToken, const char* pIgnored, const char* pTerminating) {

    char nextChar = *pBuffer;
    pToken->head = (Character*) malloc(sizeof(Character));
    Character* nextCharacter = pToken->head;
    Character* head = nextCharacter;
    nextCharacter->link = NULL;

    const char* ptrToTerminatingChar;

    while (True) {
        for (ptrToTerminatingChar = pTerminating; *ptrToTerminatingChar != '\0'; ptrToTerminatingChar++) {
            if (nextChar == *ptrToTerminatingChar)
                goto outOfLoop;
        }

        if (nextChar == '\0')
            goto outOfLoop;

        bool ignore = False;
        for (const char* ptr = pIgnored; *ptr != '\0'; ptr++) {
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

void cfg_deleteCharacters(Character* pCharacter) {

    if (pCharacter->link != NULL)
        cfg_deleteCharacters(pCharacter->link);
    free(pCharacter);
}


void cfg_deleteToken(Token* pToken) {

    cfg_deleteCharacters(pToken->head);
    free(pToken);
}