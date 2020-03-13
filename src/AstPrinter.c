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

static ReturnResult visitBinaryExprPrinter(ExprVisitor* visitor,Expr* expression);
static ReturnResult visitGroupingExprPrinter(ExprVisitor* visitor,Expr* expression);
static ReturnResult visitUnaryExprPrinter(ExprVisitor* visitor,Expr* expression);
static ReturnResult visitLiteralExprPrinter(ExprVisitor* visitor,Expr* expression);

void init_printer(AstPrinter* printer){
	printer->super.vtable.visitBinaryExpr = &visitBinaryExprPrinter;
	printer->super.vtable.visitUnaryExpr = &visitUnaryExprPrinter;
	printer->super.vtable.visitLiteralExpr = &visitLiteralExprPrinter;
	printer->super.vtable.visitGroupingExpr = &visitGroupingExprPrinter;
}

char* print(AstPrinter* printer, Expr* expression){
	   if(expression)
		  return
			 (expression->vtable.accept(expression,&printer->super)).value.string;
    return NULL;
}

static ReturnResult visitBinaryExprPrinter(ExprVisitor* visitor,Expr* expression){
	Binary * expr = (Binary*) expression;
	Expr * e_array[3];
	e_array[0] = expr->left;
	e_array[1] = expr->right;
	e_array[2] = NULL;

	return parenthesize(visitor,expr->operator->lexeme, e_array);
}
static ReturnResult visitGroupingExprPrinter(ExprVisitor* visitor,Expr* expression){
	Grouping * expr = (Grouping*) expression;
	Expr * e_array[2];
	e_array[0] = expr->expression;
	e_array[1] = NULL;
	return parenthesize(visitor,"group",e_array);

}
const static char * nil = "nil";
static ReturnResult visitUnaryExprPrinter(ExprVisitor* visitor,Expr* expression){
	Unary * expr = (Unary*) expression;
	Expr * e_array[2];
	e_array[0] = expr->right;
	e_array[1] = NULL;
	return parenthesize(visitor,expr->operator->lexeme,e_array);

}
static ReturnResult visitLiteralExprPrinter(ExprVisitor* visitor,Expr* expression){
	ReturnResult r;
	Literal * expr = (Literal*) expression;
	char * inString = NULL;
	if(expr->value == NULL || expr->value->value==NULL){
		r.value.string = strdup(nil);
		return r;
	}
	switch(expr->value->type){
		case NUMBER:
			asprintf(&inString,"%.2f", *(double*)expr->value->value);
			break;
		case TRUE:
/*			asprintf(&inString,"%.2f", *(double*)expr->value);
			break;*/
		case NIL:
/*			asprintf(&inString,"%c", *(char*)expr->value);
			break;*/
		case STRING:
		case FALSE:
		   asprintf(&inString, " %s ", (char*)expr->value->value);
/*			inString = strdup((char*)expr->value);*/
			break;
		default: break;
	}
	r.value.string = inString;
	return r;
}

ReturnResult parenthesize(ExprVisitor* visitor,char* name, Expr** expr_array){
	ReturnResult r;
	char * builder;
	Expr* temp;
	int counter;
	builder = NULL;
	temp = expr_array[0];
	asprintf(&builder, "(%s ", name);
	for(counter =0; expr_array[counter]!=NULL;counter++){
		ReturnResult other;
		other.value.string= NULL;
		temp = expr_array[counter];
		other = temp->vtable.accept(temp,visitor);
		builder = realloc(builder, sizeof(char)*(strlen(builder) +strlen(other.value.string)+1));
		strcat(builder,other.value.string);
		free(other.value.string);
		other.value.string = NULL;
/*		result = asprintf(&builder," %s", other);*/
	}
	builder = realloc(builder,sizeof(char)*(strlen(builder)+4));
	strcat(builder," ) ");
	r.value.string = builder;
	return r;
}
