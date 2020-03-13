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
#include "Lox.h"

typedef struct _Interpreter Interpreter;
struct _Interpreter {
	Visitor super;
    Lox* lox;
    void (*interpret)(Interpreter*, Expr* );
    ReturnResult (*evaluate)(Visitor*, Expr*);
    ReturnResult (*isTruthy)(Object*);
    int (*isEqual)(ReturnResult left, ReturnResult right);
    void (*checkNumberOperand)(Token* operator,Object* right);
    void (*checkNumberOperands)(Token* operator, Object* left, Object* right);
    char* (*stringify)(ReturnResult);

};

void init_Interpreter(Interpreter* intprt, Lox* lox);
void interpret(Interpreter* intprtr, Expr* expression);

ReturnResult visitLiteralExprInterpreter(Visitor* visitor, Expr* expr);
ReturnResult visitGroupingExprInterpreter(Visitor* visitor, Expr* expr);
ReturnResult visitUnaryExprInterpreter(Visitor* visitor, Expr* expr);
ReturnResult visitBinaryExprInterpreter(Visitor* visitor, Expr* expr);

ReturnResult evaluate(Visitor* visitor, Expr* expr);
ReturnResult isTruthy(Object* obj);
int isEqual(ReturnResult left, ReturnResult right);
void checkNumberOperand(Token* operator,Object* right);
void checkNumberOperands(Token* operator, Object* left, Object* right);
char* stringify(ReturnResult obj);

#endif /* INTERPRETER_H_ */
