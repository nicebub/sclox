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
/*
int main(int argc, const char* argv[]){
	AstPrinter printer;
	Token tok1,tok2;
	int one;
	double two;
	int onetype, twotype;
	char * newstr;
	Expr* binaryExpr, *unaryExpr, *literalExpr1
		, *literalExpr2, *groupingExpr;
	binaryExpr= malloc(sizeof(Binary));
	unaryExpr = malloc(sizeof(Unary));
	init_token(&tok1,MINUS, "-", NULL, 1);
	init_token(&tok2, STAR, "*", NULL, 1);
	literalExpr1 = malloc(sizeof(Literal));
	literalExpr2 = malloc(sizeof(Literal));
	one = 123;
	onetype = 0;
	two = 45.67;
	twotype = 1;
	groupingExpr = malloc(sizeof(Grouping));
	new_Literal((Literal*)literalExpr1,&one, &onetype);
	new_Literal((Literal*)literalExpr2, &two, &twotype);
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

