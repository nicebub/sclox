
#ifndef _TOKENA_H
#define _TOKENA_H
#include "Token.h"

typedef struct _TokenArray TokenArray;
struct _TokenArray {
    Token * Tokens;
    int size;
    int used;
    void (*addElementToArray)(TokenArray* array,Token* element);
    void (*deleteArray)(TokenArray* array);
    Token* (*getElementInArrayAt)(TokenArray* array,size_t index);
};
void init_TokenArray(TokenArray* array);
void addElementToTokenArray(TokenArray* array,Token* element);
void delete_TokenArray(TokenArray* array);
Token* getTokeninArrayAt(TokenArray* array,size_t index);
void initializeTokenElement(Token* arg, void* values);
#endif
