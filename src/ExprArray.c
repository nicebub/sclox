
#include <stdio.h>
#include "Token.h"
#include "ExprArray.h"
#include "memory.h"
#define INIT_SIZE 5

void init_ExprArray(ExprArray* array){
        array->Exprs = NULL;
        array->size = 0;
        array->used = 0;
	   setAllocated(array,0);
	   setCopyConstructor(array,&copyExprArray);
	   setDestructor(array,&delete_ExprArray);
        array->addElementToArray = &addElementToExprArray;
        array->getElementInArrayAt =&getExprinArrayAt;
    }
            
void addElementToExprArray(ExprArray* array,Expr* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Exprs){
                array->Exprs = new(OBJECTIVE,sizeof(Expr*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Exprs = resize(OBJECTIVE,array->Exprs,sizeof(Expr*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeExprElement(&array->Exprs[i],NULL);

            initializeExprElement(&array->Exprs[array->used],element);
            array->used++;
    }
void delete_ExprArray(void* inArray){
    ExprArray* array;
    array = (ExprArray*) inArray;
    if(array){
        int i;
        if(getReferenceCount(array)<=1){
            for(i =0; i <array->used;i++){
/*                delete(array->Exprs[i]);*/
            }
        }
        else{
            releaseReference(array);
        }
    }
}
Expr* getExprinArrayAt(ExprArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Exprs[index];
    return NULL;
}
void*  copyExprArray(void * inArr){
    ExprArray* newarr,*arr;
    int i;
    arr = (ExprArray*) inArr;
    newarr = new(OBJECTIVE,sizeof(ExprArray));
    init_ExprArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, getReference(getExprinArrayAt(arr,i)));
    }
    return newarr;
}


