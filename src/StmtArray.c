
#include <stdio.h>
#include "Token.h"
#include "StmtArray.h"
#define INIT_SIZE 5

void init_StmtArray(StmtArray* array){
        array->Stmts = NULL;
        array->size = 0;
        array->used = 0;
        array->owner_references =1;
        array->addElementToArray = &addElementToStmtArray;
        array->deleteArray = &delete_StmtArray;
        array->getElementInArrayAt =&getStmtinArrayAt;
        array->deleteArray = &delete_StmtArray;
        array->getArrayReference = &getStmtArrayReference;
        array->releaseArrayReference = &releaseStmtArrayReference;
        array->copyArray = &copyStmtArray;
    }
            
void addElementToStmtArray(StmtArray* array,Stmt* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Stmts){
                array->Stmts = malloc(sizeof(Stmt*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Stmts = realloc(array->Stmts, sizeof(Stmt*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeStmtElement(&array->Stmts[i],NULL);

            initializeStmtElement(&array->Stmts[array->used],element);
            array->used++;
    }
void delete_StmtArray(StmtArray* array){
    if(array){
        int i;
        if(array->owner_references<=1){
            for(i =0; i <array->used;i++){
                delete_Stmt(array->Stmts[i]);
            }
        }
        else{
            releaseStmtArrayReference(array);
        }
    }
}
Stmt* getStmtinArrayAt(StmtArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Stmts[index];
    return NULL;
}
StmtArray*  copyStmtArray(StmtArray * arr){
    StmtArray* newarr;
    int i;
    newarr = malloc(sizeof(StmtArray));
    init_StmtArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, getStmtinArrayAt(arr,i));
    }
    return newarr;
}
StmtArray* getStmtArrayReference(StmtArray* arr){
    arr->owner_references++;
    return arr;
}
StmtArray* releaseStmtArrayReference(StmtArray* arr){
    arr->owner_references--;
    return NULL;
}


