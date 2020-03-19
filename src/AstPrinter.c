/*
 * AstPrinter.c
 *
 *  Created on: Mar 6, 2020
 *      Author: scotty
 */

#include "AstPrinter.h"

#include "Expr.h"
#include "stdio.h"
#include "string.h"

static Object* visitBinaryExprPrinter(ExprVisitor* visitor,Expr* expression);
static Object* visitGroupingExprPrinter(ExprVisitor* visitor,Expr* expression);
static Object* visitUnaryExprPrinter(ExprVisitor* visitor,Expr* expression);
static Object* visitLiteralExprPrinter(ExprVisitor* visitor,Expr* expression);

void init_printer(AstPrinter* printer){
	printer->super.vtable.visitBinaryExpr = &visitBinaryExprPrinter;
	printer->super.vtable.visitUnaryExpr = &visitUnaryExprPrinter;
	printer->super.vtable.visitLiteralExpr = &visitLiteralExprPrinter;
	printer->super.vtable.visitGroupingExpr = &visitGroupingExprPrinter;
}

char* print(AstPrinter* printer, Expr* expression){
	   if(expression)
		  return
			 (expression->vtable.accept(expression,&printer->super))->value.string;
    return NULL;
}

static Object* visitBinaryExprPrinter(ExprVisitor* visitor,Expr* expression){
	Binary * expr = (Binary*) expression;
	Expr * e_array[3];
	e_array[0] = expr->left;
	e_array[1] = expr->right;
	e_array[2] = NULL;

	return parenthesize(visitor,expr->operator->lexeme, e_array);
}
static Object* visitGroupingExprPrinter(ExprVisitor* visitor,Expr* expression){
	Grouping * expr = (Grouping*) expression;
	Expr * e_array[2];
	e_array[0] = expr->expression;
	e_array[1] = NULL;
	return parenthesize(visitor,"group",e_array);

}
/*const static char * nil = "nil";*/
static Object* visitUnaryExprPrinter(ExprVisitor* visitor,Expr* expression){
	Unary * expr = (Unary*) expression;
	Expr * e_array[2];
	e_array[0] = expr->right;
	e_array[1] = NULL;
	return parenthesize(visitor,expr->operator->lexeme,e_array);

}
static Object* visitLiteralExprPrinter(ExprVisitor* visitor,Expr* expression){
    Object* r;
    char * inString;
    Literal * expr = (Literal*) expression;
    r = NULL;
    inString = NULL;
    r = copyObject(expr->value);
/*	if(expr->value == NULL || expr->value->value.string==NULL){
	    r = copyObject(expr->value);
		return r;
	}
	switch(expr->value->type){
		case NUMBER:
			asprintf(&inString,"%.2f", (double)expr->value->value.number);
			break;
		case TRUE:
		case NIL:
		case STRING:
		case FALSE:
		   asprintf(&inString, " %s ", (char*)expr->value->value.string);
			break;
		default: break;
	}
    r = malloc(sizeof(Object));
    init_Object(r,inString,r->type);
    r->value.string = inString;*/
	return r;
}

Object* parenthesize(ExprVisitor* visitor,char* name, Expr** expr_array){
    Object* r;
    char * builder;
    Expr* temp;
    int counter;
    r = NULL;
    builder = NULL;
    asprintf(&builder, "(%s ", name);
    for(counter =0,temp = expr_array[0]; expr_array[counter]!=NULL;counter++){
	   Object* other = NULL;
	   temp = expr_array[counter];
	   other = temp->vtable.accept(temp,visitor);
	   builder = realloc(builder, sizeof(char)*(strlen(builder) +strlen(other->value.string)+1));
	   strcat(builder,other->value.string);
	   free(other->value.string);
	   other->value.string = NULL;
	}
	builder = realloc(builder,sizeof(char)*(strlen(builder)+4));
	strcat(builder," ) ");
    r = malloc(sizeof(Object));
    init_Object(r,builder,STRING);
	r->value.string = builder;
	return r;
}
