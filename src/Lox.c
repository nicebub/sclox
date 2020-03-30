/*
 * Lox.c
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */
#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"
#include <stdio.h>
#include <stdlib.h>
#include "Lox.h"
#include "Scanner.h"
#include "Parser.h"
#include "Expr.h"
#include "Token.h"
#include "TokenType.h"
/*#include "AstPrinter.h"*/
#include "Interpreter.h"
#include "Stmt.h"
#include "Resolver.h"

static void error(Lox* lox,int line, const char* message);
static void lparse_error(Lox* lox,Token* token, const char* message);
static void runtimeError(Lox* lox, CEXCEPTION_T e);


void init_lox(Lox* lox){
    lox->interpreter = new(OBJECTIVE,sizeof(Interpreter));
    init_Interpreter(lox->interpreter,lox);
	lox->hadError = 0;
    lox->hadRuntimeError = 0;
	lox->run = &run;
	lox->runPrompt =&runPrompt;
	lox->runFile = &runFile;
	lox->report = &report;
	lox->error = &error;
	lox->parse_error = &lparse_error;
    lox->runtimeError = &runtimeError;
}
 void run(Lox* lox,char * source){
	Resolver* resolver;
/*	    init_printer(&printer);*/
	    init_Scanner(&lox->scanner, source,lox);
	    scanTokens(&lox->scanner);
	    init_Parser(&lox->parser,lox->scanner.tokens,lox);

	    lox->statements = (StmtArray*)parse(&lox->parser);

	    if(lox->hadError)
	    	return;
	resolver = new(OBJECTIVE,sizeof(Resolver));
	init_Resolver(resolver,lox->interpreter);
	resolver->resolve(resolver,lox->statements);
	if(lox->hadError)
		return;
	   lox->interpreter->interpret(lox->interpreter,lox->statements);
	    /* TODO need to delete expression potentially after printing */
/*	    delete_StmtArray(statements);
	    statements = NULL;*/
/*	    delete_scanner(&lox->scanner);*/
/*	    delete_parser(&lox->parser);*/
/*	    init_scanner(&scanner, source,lox);*/
/*	    delete_TokenArray(&scanner.tokens);*/
	    init_TokenArray(lox->scanner.tokens);
}

 void runFile(Lox* lox,const char * file){
	FILE* inFile;
	char* line, *templine;
	size_t capp;
	inFile = NULL;
	line = NULL;
	capp = 0;

	if(!file){
		fprintf(stderr,"file not given\n");
		exit( EXIT_FAILURE);
	}
	inFile = fopen(file, "r");
	if(!inFile){
		perror("Error");
		exit( EXIT_FAILURE);
	}
	templine = malloc(sizeof(char)*2);
	memset(templine,0,sizeof(char)*2);
	while((getline(&line,&capp,inFile)) > 0){
	    templine = realloc(templine,sizeof(char)*(strlen(templine)+strlen(line)+1));
	    strncat(templine,line,strlen(line));
	}
	free(line);
	line = templine;
	 run(lox,line);
	 free(line);
	 line = NULL;
	
	 if(lox->hadError)
	   exit(65);
	if(lox->hadRuntimeError)
	    exit(70);
}

 void runPrompt(Lox* lox){
	char* line;
	size_t lcap = 0;
	line = NULL;
	for(;;){
		printf("> ");
	    getline(&line,&lcap,stdin);
	    if(line && strcmp(line,"") != 0){
		   run(lox,line);
		   free(line);
		   line = NULL;
		   lox->hadError = 0;
	    }
	    else{
		   free(line);
		   line = NULL;
		   break;
	    }
	}
}

 void report(Lox* lox,int line, const char * where, const char * message){
	fprintf(stderr,"[line %d] Error %s: %s\n",line,where,message);
	lox->hadError = 1;
}

 void error(Lox* lox,int line, const char* message){
	report(lox,line,"",message);
}
static void lparse_error(Lox* lox,Token* token, const char* message){
	if(token->super.type == EEOF){
		report(lox, token->line, "at end", message);
	}
	else{
		char * str = NULL;
		asprintf(&str," at '%s'",token->lexeme);
		report(lox, token->line, str,message);
		free(str);
		str = NULL;
	}
}
static void runtimeError(Lox* lox, CEXCEPTION_T e){
    fprintf(stderr,"%s\n[line %d] Runtime Error\n",e.message,e.token->line);
    lox->hadRuntimeError = 1;
}
