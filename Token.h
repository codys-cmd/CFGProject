#ifndef TOKEN_H
#define TOKEN_H

//Boolean definitions.
typedef int bool;
#define True 1
#define False 0

typedef struct Character Character;

struct Character {
    char data;
    Character* link;
};

/*
    Represents a linked list of characters, and the character
    at the end of it.
*/
typedef struct {
    char terminatingChar;
    Character* head;
} Token;

/*
    Returns whether or not token matches a keyword.
    Returns true if it does, false if it doesn't.
*/
bool cfg_isTokenKeyword(
    Character* pHead, //Pointer to head of token.
    char* pKeyword    //C string representing keyword.
    );

/*
    Returns whether or not token contains a keyword.
    Returns true if it does, false if it doesn't.
*/
bool cfg_tokenContainsKeyword(
    Character* pHead, //Pointer to head of token.
    char* pKeyword    //C string representing keyword.
    );

/*
    Returns if two tokens match (not counting terminating characters).
    Returns true if they do, false if they don't.
*/
bool cfg_tokensMatch(
    Character* pHeadA, //Pointer to the head of the first token.
    Character* pHeadB  //Pointer to the head of the second token.
    );

#endif