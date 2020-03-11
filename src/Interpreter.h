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
typedef struct _Interpreter Interpreter;
struct _Interpreter {
	Visitor super;
};

void init_Interpreter(Interpreter* intprt);

ReturnResult visitLiteralExprInterpreter(Visitor* visitor, Expr* expr);
ReturnResult visitGroupingExprInterpreter(Visitor* visitor, Expr* expr);
ReturnResult visitUnaryExprInterpreter(Visitor* visitor, Expr* expr);

ReturnResult evaluate(Visitor* visitor, Expr* expr);
ReturnResult isTruthy(void* obj);

#endif /* INTERPRETER_H_ */
