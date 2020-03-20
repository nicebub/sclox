/*
 ============================================================================
 Name        : sclox.c
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Lox.h"

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
/*    deleteEnvironment(lox.interpreter.environment);*/
	return EXIT_SUCCESS;
}

