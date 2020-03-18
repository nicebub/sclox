
#include <stdio.h>
#include "Token.h"
#include "TokenArray.h"
#define INIT_SIZE 5

void init_TokenArray(TokenArray* array){
        array->Tokens = NULL;
        array->size = 0;
        array->used = 0;
        array->addElementToArray = &addElementToTokenArray;
        array->deleteArray = &delete_TokenArray;
        array->getElementInArrayAt =&getTokeninArrayAt;
    }
            
void addElementToTokenArray(TokenArray* array,Token* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Tokens){
                array->Tokens = malloc(sizeof(Token) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Tokens = realloc(array->Tokens, sizeof(Token)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeTokenElement(&array->Tokens[i],NULL);

            initializeTokenElement(&array->Tokens[array->used],element);
            array->used++;
    }
void delete_TokenArray(TokenArray* array){
    if(array){
        int i;
        for(i =0; i <array->used;i++){
            delete_Token(&array->Tokens[i]);
        }
    }
}
Token* getTokeninArrayAt(TokenArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return &array->Tokens[index];
    return NULL;
}
