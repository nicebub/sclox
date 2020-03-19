/*
 * Interpreter.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Expr.h"
#include "Object.h"
#include "Stmt.h"
#include "additions.h"
#include "Environment.h"
#ifndef _STMTARRAY
#define _STMTARRAY
	typedef struct _StmtArray StmtArray;
	extern delete_StmtArray(StmtArray* array);
#endif
#include "StmtArray.h"
#include "ExprArray.h"
typedef struct _Interpreter Interpreter;
typedef struct _SuperVisitor SuperVisitor;
struct _Interpreter {
	StmtVisitor super;
    Environment* globals;
    Environment* environment;
    void* lox;
    void (*interpret)(Interpreter*, StmtArray* );
    Object* (*evaluate)(ExprVisitor*, Expr*);
    Object* (*isTruthy)(Object*);
    int (*isEqual)(Object* left, Object* right);
    void (*checkNumberOperand)(Token* operator,Object* right);
    void (*checkNumberOperands)(Token* operator, Object* left, Object* right);
    char* (*stringify)(Object*);

};

void init_Interpreter(Interpreter* intprt, void* lox);
void interpret(Interpreter* intprtr, StmtArray* array);
void execute(Interpreter* intprtr, Stmt* stmt);
void executeBlock(Interpreter* intrprtr ,StmtArray* array,Environment* newenv);



int isEqual(Object* left, Object* right);
void checkNumberOperand(Token* operator,Object* right);
void checkNumberOperands(Token* operator, Object* left, Object* right);
char* stringify(Object* obj);

#endif /* INTERPRETER_H_ */
