/*
 * AstPrinter.h
 *
 *  Created on: Mar 6, 2020
 *      Author: scotty
 */

#ifndef ASTPRINTER_H_
#define ASTPRINTER_H_

#include "Expr.h"
typedef struct _AstPrinter AstPrinter;
struct _AstPrinter {
	Visitor super;
};

void init_printer(AstPrinter* printer);
char* print(AstPrinter* printer,Expr* expression);

static ReturnResult visitBinaryExprPrinter(Visitor* visitor,Expr* expression);
static ReturnResult visitGroupingExprPrinter(Visitor* visitor,Expr* expression);
static ReturnResult visitUnaryExprPrinter(Visitor* visitor,Expr* expression);
static ReturnResult visitLiteralExprPrinter(Visitor* visitor,Expr* expression);

ReturnResult parenthesize(Visitor* visitor,char* name, Expr** expr_array);

#endif /* ASTPRINTER_H_ */
