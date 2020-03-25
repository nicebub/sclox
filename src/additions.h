#ifndef _ADDITIONS_H
#define _ADDITIONS_H

#include "StmtArray.h"
#include "Stmt.h"
#include "TokenArray.h"
void initializeStmtElement(Stmt** statement, void* values);
void initializeExprElement(Expr** exprs, void * values);
void initializeObjectElement(Object** objs, void * values);
void initializememoryElement(Object** objs, void * values);
#endif
