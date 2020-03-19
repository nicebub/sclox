
#ifndef _STMTA_H
#define _STMTA_H
#include "Token.h"
#include "Stmt.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(ExprArray* array);
#endif


struct _StmtArray {
    Stmt ** Stmts;
    int owner_references;
    int size;
    int used;
    void (*addElementToArray)(StmtArray* array,Stmt* element);
    void (*deleteArray)(StmtArray* array);
    Stmt* (*getElementInArrayAt)(StmtArray* array,size_t index);
    StmtArray*  (*copyArray)(StmtArray * arr);
    StmtArray* (*getArrayReference)(StmtArray* arr);
    StmtArray* (*releaseArrayReference)(StmtArray* arr);
    
};
void init_StmtArray(StmtArray* array);
void addElementToStmtArray(StmtArray* array,Stmt* element);
void delete_StmtArray(StmtArray* array);
Stmt* getStmtinArrayAt(StmtArray* array,size_t index);
void initializeStmtElement(Stmt** arg, void* values);
StmtArray*  copyStmtArray(StmtArray * arr);
StmtArray* getStmtArrayReference(StmtArray* arr);
StmtArray* releaseStmtArrayReference(StmtArray* arr);
#endif
