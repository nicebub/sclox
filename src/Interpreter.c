/*
 * Interpreter.c
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#include "Interpreter.h"
#include "Expr.h"
#include "TokenType.h"
#include "ReturnResult.h"

void init_Interpreter(Interpreter* intprtr){
	intprtr->super.vtable.visitLiteralExpr = &visitLiteralExprInterpreter;
	intprtr->super.vtable.visitGroupingExpr = &visitGroupingExprInterpreter;
	intprtr->super.vtable.visitUnaryExpr = &visitUnaryExprInterpreter;
}
ReturnResult visitLiteralExprInterpreter(Visitor* visitor, Expr* expr){
	ReturnResult r;
	switch(r.type){
	case NUMBER:
		r.value.number = *(double*)((Literal*)expr)->value->value;
		break;
	case STRING:
	case TRUE:
	case FALSE:
	case NIL:
		r.value.string = ((Literal*)expr)->value->value;
		break;
	default: break;
	}
	return r;
}
ReturnResult visitGroupingExprInterpreter(Visitor* visitor, Expr* expr){
	return evaluate(visitor,((Grouping*)expr)->expression);
}
ReturnResult visitUnaryExprInterpreter(Visitor* visitor, Expr* expr){
	ReturnResult right,result;
	right = evaluate(visitor,((Unary*)expr)->right);
/*
	switch(((Unary*)expr)->operator->type){
		case BANG:
			return (void*)!isTruthy(((Unary*)expr)->right);
		case MINUS:
			return (void*)-*(double*)((Unary*)expr)->right;
	}*/
	return result;
}

ReturnResult evaluate(Visitor* visitor, Expr* expr){
	return expr->vtable.accept(expr,visitor);
}
ReturnResult isTruthy(void* obj){
	ReturnResult r;
/*	if( obj == NULL) return 0;*/
/*TODO	if( obj->isbool) return obj;*/
/*	return (void*)1;*/
	return r;
}
