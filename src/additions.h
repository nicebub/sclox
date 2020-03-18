#ifndef _ADDITIONS_H
#define _ADDITIONS_H

#include "StmtArray.h"
#include "Stmt.h"

void initializeStmtElement(Stmt** statement, void* values);
void initializeExprElement(Expr** exprs, void * values);
void initializeReturnResultElement(ReturnResult* exprs, void * values);
#endif
