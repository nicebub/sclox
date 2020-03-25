
#include <stdio.h>
#include "Token.h"
#include "TokenArray.h"
#include "memory.h"
#define INIT_SIZE 5

void init_TokenArray(TokenArray* array){
        array->Tokens = NULL;
        array->size = 0;
        array->used = 0;
/*	   array->memory.allocated = 0;*/
/*	   array->memory.new*/
    setAllocated(array,0);
    setCopyConstructor(array,&copyTokenArray);
    setDestructor(array,&delete_TokenArray);

        array->addElementToArray = &addElementToTokenArray;
        array->delete = &delete_TokenArray;
        array->getElementInArrayAt =&getTokeninArrayAt;
        array->copy = &copyTokenArray;
    }
            
void addElementToTokenArray(TokenArray* array,Token* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Tokens){
                array->Tokens = new(OBJECTIVE,sizeof(Token) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Tokens = resize(OBJECTIVE,array->Tokens, sizeof(Token)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
			 initializeTokenElement(&array->Tokens[i],NULL);
           initializeTokenElement(&array->Tokens[array->used],element);
            array->used++;
    }
void delete_TokenArray(void* inArray){
    TokenArray* array = (TokenArray*) inArray;
    if(array){
        int i;
        if(getReferenceCount(array)<=1){
            for(i =0; i <array->used;i++){
                delete(&array->Tokens[i]);
            }
		  delete(array);
		  array = NULL;
        }
        else{
            releaseReference(array);
        }
    }
}
Token* getTokeninArrayAt(TokenArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Tokens[index];
    return NULL;
}
void*  copyTokenArray(void * inArr){
    TokenArray* arr;
    TokenArray* newarr;
    int i;
    arr = (TokenArray*) inArr;
    newarr = new(OBJECTIVE, sizeof(TokenArray));
    init_TokenArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, copy(getTokeninArrayAt(arr,i)));
    }
    setAllocated(newarr,1);
    return newarr;
}


