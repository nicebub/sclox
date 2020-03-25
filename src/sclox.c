/*
 ============================================================================
 Name        : sclox.c
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Lox.h"
#include "memory.h"
int main(int argc, const char* argv[] ) {
	Lox *lox;
    lox = new(OBJECTIVE, sizeof(Lox));
/*	char * newarray;*/
	init_lox(lox);
/*	newarray = new(RAW,sizeof(char)*10);*/
/*	delete(newarray);*/
/*	return 0;*/
	if(argc >2){
		printf("Usage: sclox [script]\n");
		return EXIT_SUCCESS;
	}
	else if(argc == 2)
		runFile(lox,argv[1]);
	else
		runPrompt(lox);
/*    deleteEnvironment(lox.interpreter.environment);*/
	return EXIT_SUCCESS;
}

