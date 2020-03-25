
#ifndef _STMTA_H
#define _STMTA_H
#include "Token.h"
#include "Stmt.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(void* array);
#endif


struct _StmtArray {
    Stmt ** Stmts;
    int size;
    int used;
    void (*addElementToArray)(StmtArray* array,Stmt* element);
    void (*delete)(void* array);
    Stmt* (*getElementInArrayAt)(StmtArray* array,size_t index);
    void*  (*copy)(void * arr);
    
};
void init_StmtArray(StmtArray* array);
void addElementToStmtArray(StmtArray* array,Stmt* element);
void delete_StmtArray(void* array);
Stmt* getStmtinArrayAt(StmtArray* array,size_t index);
void initializeStmtElement(Stmt** arg, void* values);
void*  copyStmtArray(void * arr);
#endif
