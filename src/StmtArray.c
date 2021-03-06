
#include <stdio.h>
#include "Token.h"
#include "StmtArray.h"
#define INIT_SIZE 5

void init_StmtArray(StmtArray* array){
        array->Stmts = NULL;
        array->size = 0;
        array->used = 0;
	   setAllocated(array,0);
	   setCopyConstructor(array,&copyStmtArray);
	   setDestructor(array,&delete_StmtArray);
        array->addElementToArray = &addElementToStmtArray;
        array->getElementInArrayAt =&getStmtinArrayAt;
        array->delete = &delete_StmtArray;
        array->copy = &copyStmtArray;
    }
            
void addElementToStmtArray(StmtArray* array,Stmt* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Stmts){
                array->Stmts = new(OBJECTIVE,sizeof(Stmt*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Stmts = resize(OBJECTIVE,array->Stmts, sizeof(Stmt*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeStmtElement(&array->Stmts[i],NULL);

            initializeStmtElement(&array->Stmts[array->used],element);
            array->used++;
    }
void delete_StmtArray(void* inArray){
    StmtArray* array;
    array = (StmtArray*) inArray;
    if(array){
        int i;
        if(getReferenceCount(array)<=1){
            for(i =0; i <array->used;i++){
                delete(array->Stmts[i]);
            }
        }
        else{
            releaseReference(array);
        }
    }
}
Stmt* getStmtinArrayAt(StmtArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Stmts[index];
    return NULL;
}
void*  copyStmtArray(void * inArr){
    StmtArray* newarr,*arr;
    int i;
    arr = (StmtArray*)inArr;
    newarr = new(OBJECTIVE,sizeof(StmtArray));
    init_StmtArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, copy(getStmtinArrayAt(arr,i)));
    }
    setAllocated(newarr,1);
    return newarr;
}


