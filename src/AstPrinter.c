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
void init_printer(AstPrinter* printer){
	printer->super.vtable.visitBinaryExpr = &visitBinaryExprPrinter;
	printer->super.vtable.visitUnaryExpr = &visitUnaryExprPrinter;
	printer->super.vtable.visitLiteralExpr = &visitLiteralExprPrinter;
	printer->super.vtable.visitGroupingExpr = &visitGroupingExprPrinter;
}

char* print(AstPrinter* printer, Expr* expression){
	return expression->vtable.accept(expression,&printer->super);
}

static void * visitBinaryExprPrinter(Visitor* visitor,Expr* expression){
	Binary * expr = (Binary*) expression;
	Expr * e_array[3];
	e_array[0] = expr->left;
	e_array[1] = expr->right;
	e_array[2] = NULL;
	return (void*) parenthesize(visitor,expr->operator->lexeme, e_array);
}
static void * visitGroupingExprPrinter(Visitor* visitor,Expr* expression){
	Grouping * expr = (Grouping*) expression;
	Expr * e_array[2];
	e_array[0] = expr->expression;
	e_array[1] = NULL;
	return (void*)parenthesize(visitor,"group",e_array);

}
const static char * nil = "nil";
static void * visitUnaryExprPrinter(Visitor* visitor,Expr* expression){
	Unary * expr = (Unary*) expression;
	Expr * e_array[2];
	e_array[0] = expr->right;
	e_array[1] = NULL;
	return (void*)parenthesize(visitor,expr->operator->lexeme,e_array);

}
static void * visitLiteralExprPrinter(Visitor* visitor,Expr* expression){
	Literal * expr = (Literal*) expression;
	char * inString = NULL;

	if(expr->value == NULL) return (void*)nil;
	switch(*expr->valueType){
		case 0:
			asprintf(&inString,"%d", *(int*)expr->value);
			break;
		case 1:
			asprintf(&inString,"%.2f", *(double*)expr->value);
			break;
		case 2:
			asprintf(&inString,"%c", *(char*)expr->value);
			break;
		case 3:
			inString = strdup((char*)expr->value);
			break;
	}
	return inString;
}

char * parenthesize(Visitor* visitor,char* name, Expr** expr_array){
	char * builder;
	Expr* temp;
	int counter;
	builder = NULL;
	temp = expr_array[0];
	asprintf(&builder, "(%s ", name);
	for(counter =0; expr_array[counter]!=NULL;counter++){
		char* other = NULL;
		temp = expr_array[counter];
		other = (char*)temp->vtable.accept(temp,visitor);
		builder = realloc(builder, sizeof(char)*(strlen(builder) +strlen(other)+1));
		strcat(builder,other);
		free(other);
		other = NULL;
/*		result = asprintf(&builder," %s", other);*/
	}
	builder = realloc(builder,sizeof(char)*(strlen(builder)+3));
	strcat(builder,") ");
	return builder;
}
