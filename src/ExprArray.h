
#ifndef _EXPRA_H
#define _EXPRA_H
#include "Token.h"
#include "Expr.h"

#ifndef _STMTARRAY
#define _STMTARRAY
    typedef struct _StmtArray StmtArray;
    extern void delete_StmtArray(StmtArray* array);
#endif
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(ExprArray* array);
#endif
struct _ExprArray {
    Expr ** Exprs;
    int size;
    int used;
    void (*addElementToArray)(ExprArray* array,Expr* element);
    void (*deleteArray)(ExprArray* array);
    Expr* (*getElementInArrayAt)(ExprArray* array,size_t index);
};
void init_ExprArray(ExprArray* array);
void addElementToExprArray(ExprArray* array,Expr* element);

Expr* getExprinArrayAt(ExprArray* array,size_t index);
void initializeExprElement(Expr** arg, void* values);
#endif
