
#ifndef _EXPRA_H
#define _EXPRA_H
#include "Token.h"
#include "Expr.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(void* array);
#endif


struct _ExprArray {
    Expr ** Exprs;
    int size;
    int used;
    void (*addElementToArray)(ExprArray* array,Expr* element);
    Expr* (*getElementInArrayAt)(ExprArray* array,size_t index);
    
};
void init_ExprArray(ExprArray* array);

void addElementToExprArray(ExprArray* array,Expr* element);
Expr* getExprinArrayAt(ExprArray* array,size_t index);

void initializeExprElement(Expr** arg, void* values);

void*  copyExprArray(void * arr);
void delete_ExprArray(void* array);
#endif
