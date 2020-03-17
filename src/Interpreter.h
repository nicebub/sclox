/*
 * Interpreter.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Expr.h"
#include "ReturnResult.h"
#include "Object.h"
#include "Stmt.h"
#include "additions.h"
#include "Environment.h"
typedef struct _Interpreter Interpreter;
typedef struct _SuperVisitor SuperVisitor;
struct _Interpreter {
	StmtVisitor super;
	Environment* environment;
    void* lox;
    void (*interpret)(Interpreter*, StmtArray* );
    ReturnResult (*evaluate)(ExprVisitor*, Expr*);
    ReturnResult (*isTruthy)(ReturnResult);
    int (*isEqual)(ReturnResult left, ReturnResult right);
    void (*checkNumberOperand)(Token* operator,ReturnResult right);
    void (*checkNumberOperands)(Token* operator, ReturnResult left, ReturnResult right);
    char* (*stringify)(ReturnResult);

};

void init_Interpreter(Interpreter* intprt, void* lox);
void interpret(Interpreter* intprtr, StmtArray* array);
void execute(Interpreter* intprtr, Stmt* stmt);
void executeBlock(Interpreter* intrprtr ,StmtArray* array,Environment* newenv);



int isEqual(ReturnResult left, ReturnResult right);
void checkNumberOperand(Token* operator,ReturnResult right);
void checkNumberOperands(Token* operator, ReturnResult left, ReturnResult right);
char* stringify(ReturnResult obj);

#endif /* INTERPRETER_H_ */
