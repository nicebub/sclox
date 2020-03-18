
#include <stdio.h>
#include "Token.h"
#include "ExprArray.h"
#define INIT_SIZE 5

void init_ExprArray(ExprArray* array){
        array->Exprs = NULL;
        array->size = 0;
        array->used = 0;
        array->addElementToArray = &addElementToExprArray;
        array->deleteArray = &delete_ExprArray;
        array->getElementInArrayAt =&getExprinArrayAt;
    }
            
void addElementToExprArray(ExprArray* array,Expr* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Exprs){
                array->Exprs = malloc(sizeof(Expr*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Exprs = realloc(array->Exprs, sizeof(Expr*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeExprElement(&array->Exprs[i],NULL);

            initializeExprElement(&array->Exprs[array->used],element);
            array->used++;
    }
void delete_ExprArray(ExprArray* array){
    if(array){
        int i;
        for(i =0; i <array->used;i++){
            delete_Expr(array->Exprs[i]);
        }
    }
}
Expr* getExprinArrayAt(ExprArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Exprs[index];
    return NULL;
}
