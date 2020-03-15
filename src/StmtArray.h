
#ifndef _STMTA_H
#define _STMTA_H
#include "Token.h"
#include "Stmt.h"
typedef struct _StmtArray StmtArray;
struct _StmtArray {
    Stmt* * Stmts;
    int size;
    int used;
    void (*addElementToArray)(StmtArray* array,Stmt* element);
    void (*deleteArray)(StmtArray* array);
    Stmt* (*getElementInArrayAt)(StmtArray* array,size_t index);
};
void init_StmtArray(StmtArray* array);
void addElementToStmtArray(StmtArray* array,Stmt* element);
void deleteStmtArray(StmtArray* array);
Stmt* getStmtinArrayAt(StmtArray* array,size_t index);
void initializeStmtElement(Stmt** arg, void* values);
#endif