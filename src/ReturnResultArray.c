
#include <stdio.h>
#include "Token.h"
#include "ReturnResult.h"
#include "ReturnResultArray.h"
#define INIT_SIZE 5

void init_ReturnResultArray(ReturnResultArray* array){
        array->ReturnResults = NULL;
        array->size = 0;
        array->used = 0;
        array->addElementToArray = &addElementToReturnResultArray;
        array->deleteArray = &delete_ReturnResultArray;
        array->getElementInArrayAt =&getReturnResultinArrayAt;
    }
            
void addElementToReturnResultArray(ReturnResultArray* array,ReturnResult* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->ReturnResults){
                array->ReturnResults = malloc(sizeof(ReturnResult) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->ReturnResults = realloc(array->ReturnResults, sizeof(ReturnResult)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeReturnResultElement(&array->ReturnResults[i],NULL);

            initializeReturnResultElement(&array->ReturnResults[array->used],element);
            array->used++;
    }
void delete_ReturnResultArray(ReturnResultArray* array){
    if(array){
        int i;
        for(i =0; i <array->used;i++){
            free(&array->ReturnResults[i].value);
        }
    }
}
ReturnResult* getReturnResultinArrayAt(ReturnResultArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return &array->ReturnResults[index];
    return NULL;
}
