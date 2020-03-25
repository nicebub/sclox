
#ifndef _TOKENA_H
#define _TOKENA_H
#include "Token.h"

typedef struct _TokenArray TokenArray;

struct _TokenArray {
    Token ** Tokens;
    int size;
    int used;
    void (*addElementToArray)(TokenArray* array,Token* element);
    void (*delete)(void* array);
    Token* (*getElementInArrayAt)(TokenArray* array,size_t index);
    void*  (*copy)(void * arr);
    
};
void init_TokenArray(TokenArray* array);
void addElementToTokenArray(TokenArray* array,Token* element);
void delete_TokenArray(void* array);
Token* getTokeninArrayAt(TokenArray* array,size_t index);
void initializeTokenElement(Token** arg, void* values);
void*  copyTokenArray(void * arr);
#endif
