
#include <stdio.h>
#include "Token.h"
#include "TokenArray.h"
#define INIT_SIZE 5

void init_TokenArray(TokenArray* array){
        array->Tokens = NULL;
        array->size = 0;
        array->used = 0;
        array->owner_references =1;
        array->addElementToArray = &addElementToTokenArray;
        array->deleteArray = &delete_TokenArray;
        array->getElementInArrayAt =&getTokeninArrayAt;
        array->deleteArray = &delete_TokenArray;
        array->getArrayReference = &getTokenArrayReference;
        array->releaseArrayReference = &releaseTokenArrayReference;
        array->copyArray = &copyTokenArray;
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
        if(array->owner_references<=1){
            for(i =0; i <array->used;i++){
                delete_Token(&array->Tokens[i]);
            }
        }
        else{
            releaseTokenArrayReference(array);
        }
    }
}
Token* getTokeninArrayAt(TokenArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return &array->Tokens[index];
    return NULL;
}
TokenArray*  copyTokenArray(TokenArray * arr){
    TokenArray* newarr;
    int i;
    newarr = malloc(sizeof(TokenArray));
    init_TokenArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, getTokeninArrayAt(arr,i));
    }
    return newarr;
}
TokenArray* getTokenArrayReference(TokenArray* arr){
    arr->owner_references++;
    return arr;
}
TokenArray* releaseTokenArrayReference(TokenArray* arr){
    arr->owner_references--;
    return NULL;
}


