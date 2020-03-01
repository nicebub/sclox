/*
 * Lox.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */
#include <stdio.h>
#include <stdlib.h>
#include "Lox.h"
#include "Scanner.h"
#include "Token.h"
#include "TokenType.h"

void init_lox(Lox* lox){
	lox->hadError = 0;
	lox->run = &run;
	lox->runPrompt =&runPrompt;
	lox->runFile = &runFile;
	lox->report = &report;
	lox->error = &error;

}
 void run(Lox* lox,char * source){
	    Scanner scanner;
	    init_scanner(&scanner, source);
//	    init_tokenArray(&scanner.tokens);
	    scanTokens(lox,&scanner);
	    // For now, just print the tokens.
	    for(int i = 0;i<scanner.tokens.used;i++)
	    	printf("%s\n",token_toString(&scanner.tokens.tokens[i]));
	    delete_tokenArray(&scanner.tokens);
	    init_tokenArray(&scanner.tokens);
}

 void runFile(Lox* lox,const char * file){
	FILE* inFile;
	char* line;
	size_t lread;
	inFile = NULL;
	line = NULL;
	lread = 0;

	if(!file){
		printf("file not given\n");
		exit( EXIT_FAILURE);
	}
	inFile = fopen(file, "r");
	if(!inFile){
		perror("Error");
		exit( EXIT_FAILURE);
	}
	while((lread = getline(&line,0,inFile)) > 0)
		run(lox,line);
	 if(lox->hadError)
		 exit(65);
}

 void runPrompt(Lox* lox){
	char* line = NULL;
	size_t written = 0;
	size_t lcap = 0;
	for(;;){
		printf("> ");
		written = getline(&line,&lcap,stdin);
		line[written-1] = '\0';
		if(line[written-2] == '\r')
			line[written-2] = '\0';
		run(lox,line);
		free(line);
		line = NULL;
		lox->hadError = 0;
	}
}

 void report(Lox* lox,int line, const char * where, const char * message){
	fprintf(stderr,"[line %d] Error %s:%s\n",line,where,message);
	lox->hadError = 1;
}
 void error(Lox* lox,int line, const char* message){
	report(lox,line,"",message);
}
