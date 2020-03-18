
#ifndef _STMTA_H
#define _STMTA_H
#include "Token.h"
#include "Stmt.h"

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
struct _StmtArray {
    Stmt ** Stmts;
    int size;
    int used;
    void (*addElementToArray)(StmtArray* array,Stmt* element);
    void (*deleteArray)(StmtArray* array);
    Stmt* (*getElementInArrayAt)(StmtArray* array,size_t index);
};
void init_StmtArray(StmtArray* array);
void addElementToStmtArray(StmtArray* array,Stmt* element);
Stmt* getStmtinArrayAt(StmtArray* array,size_t index);
void initializeStmtElement(Stmt** arg, void* values);
#endif
