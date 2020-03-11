/*
 ============================================================================
 Name        : sclox.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Lox.h"
#include "Expr.h"
#include "TokenType.h"
#include "AstPrinter.h"
#include "Object.h"
/*
int main(int argc, const char* argv[]){
	AstPrinter printer;
	Token tok1,tok2;
	double one,two;
	TokenType onetype, twotype;
	char * newstr;
	Expr* binaryExpr, *unaryExpr, *literalExpr1
		, *literalExpr2, *groupingExpr;
	Object* obj1, *obj2;
	tok1.literal = NULL;
	tok2.literal = NULL;
	binaryExpr= malloc(sizeof(Binary));
	unaryExpr = malloc(sizeof(Unary));
	init_token(&tok1,MINUS, "-", NULL, 1);
	init_token(&tok2, STAR, "*", NULL, 1);
	literalExpr1 = malloc(sizeof(Literal));
	literalExpr2 = malloc(sizeof(Literal));
	one = 123;
	onetype = NUMBER;
	two = 45.67;
	twotype = NUMBER;
	groupingExpr = malloc(sizeof(Grouping));
	obj1 = malloc(sizeof(Object));
	obj2 = malloc(sizeof(Object));
	init_Object(obj1,&one,onetype);
	init_Object(obj2,&two,twotype);

	new_Literal((Literal*)literalExpr1,obj1);
	new_Literal((Literal*)literalExpr2,obj2);
	new_Grouping((Grouping*)groupingExpr, literalExpr2);
	new_Unary((Unary*)unaryExpr, &tok1, literalExpr1);
	new_Binary((Binary*)binaryExpr, unaryExpr, &tok2, groupingExpr);
	init_printer(&printer);
	newstr = print(&printer,binaryExpr);
	printf("%s\n", newstr);
	free(newstr);
	newstr = NULL;
}
*/

int main(int argc, const char* argv[] ) {
	Lox lox;
	init_lox(&lox);
	if(argc >2){
		printf("Usage: sclox [script]\n");
		return EXIT_SUCCESS;
	}
	else if(argc == 2)
		runFile(&lox,argv[1]);
	else
		runPrompt(&lox);

	return EXIT_SUCCESS;
}

