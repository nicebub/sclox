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
	ExprVisitor super;
};

void init_printer(AstPrinter* printer);
char* print(AstPrinter* printer,Expr* expression);

Object* parenthesize(ExprVisitor* visitor,char* name, Expr** expr_array);

#endif /* ASTPRINTER_H_ */
