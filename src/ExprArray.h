
#ifndef _EXPRA_H
#define _EXPRA_H
#include "Token.h"
#include "Expr.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(ExprArray* array);
#endif


struct _ExprArray {
    Expr ** Exprs;
    int owner_references;
    int size;
    int used;
    void (*addElementToArray)(ExprArray* array,Expr* element);
    void (*deleteArray)(ExprArray* array);
    Expr* (*getElementInArrayAt)(ExprArray* array,size_t index);
    ExprArray*  (*copyArray)(ExprArray * arr);
    ExprArray* (*getArrayReference)(ExprArray* arr);
    ExprArray* (*releaseArrayReference)(ExprArray* arr);
    
};
void init_ExprArray(ExprArray* array);
void addElementToExprArray(ExprArray* array,Expr* element);
void delete_ExprArray(ExprArray* array);
Expr* getExprinArrayAt(ExprArray* array,size_t index);
void initializeExprElement(Expr** arg, void* values);
ExprArray*  copyExprArray(ExprArray * arr);
ExprArray* getExprArrayReference(ExprArray* arr);
ExprArray* releaseExprArrayReference(ExprArray* arr);
#endif
