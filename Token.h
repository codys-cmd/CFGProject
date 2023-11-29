#ifndef TOKEN_H
#define TOKEN_H

//Boolean definitions.
typedef int bool;
#define True 1
#define False 0

//Grammar Keywords --------------
extern const char* If_KW;
extern const char* While_KW;
extern const char* Else_KW;

extern const char* True_KW;
extern const char* False_KW;

extern const char* ParenL_KW;
extern const char* ParenR_KW;
extern const char* SquigL_KW;
extern const char* SquigR_KW;

extern const char* Assign_KW;
extern const char* Equals_KW;
extern const char* LessTh_KW;
extern const char* MoreTh_KW;
extern const char* Inc_KW;
extern const char* Dec_KW;
extern const char* FloatPnt_KW;

extern const char* Power_KW;
extern const char* Plus_KW;
extern const char* Minus_KW;
extern const char* Mult_KW;
extern const char* Divide_KW;
//-------------------------------

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