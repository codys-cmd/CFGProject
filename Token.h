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

/*
    Returns whether or not token starts with a keyword or not.
    Returns true if it does, false otherwise.
*/
bool cfg_tokenStartsWith(
    Character* pHead, //Pointer to the head of the token.
    char* pKeyword    //C string representing keyword.
    );

/*
    Returns pointer to character after a certain index.
*/
Character* cfg_returnCharsAfterIndex(
    Character* pHead, //Pointer to the head of the token.
    int index         //Index to start looking for keyword (0 indexed).
    );

/*
    Converts a token to a C string.
    Returns a heap-allocated c string.
*/
char* cfg_tokenToCString(
    Character* pHead //Pointer to the head of the token.
    );

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
    );

/*
    Deletes a token (frees it's memory).
    Handles deleting it's character pointers as well.
*/
void cfg_deleteToken(
    Token* pToken //Pointer to token.
    );

#endif