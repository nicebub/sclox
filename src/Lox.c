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
#include "AstPrinter.h"
#include "Interpreter.h"
#include "Stmt.h"

static void error(Lox* lox,int line, const char* message);
static void lparse_error(Lox* lox,Token* token, const char* message);
static void runtimeError(Lox* lox, CEXCEPTION_T e);


void init_lox(Lox* lox){
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
	    Scanner scanner;
	    Parser parser;
	    Expr *expression;
	    StmtArray* statements;
	    AstPrinter printer;
	    Interpreter interpreter;
/*	    char * str;*/
/*	    int i;*/

	    init_printer(&printer);
	    init_scanner(&scanner, source,lox);
/*	    init_tokenArray(&scanner.tokens);*/
	    scanTokens(&scanner);
	    init_parser(&parser,&scanner.tokens,lox);
/*	    init_StmtArray(&statements);*/
	    statements = (StmtArray*)parse(&parser);

	    if(lox->hadError)
	    	return;
	    init_Interpreter(&interpreter,lox);
	    interpreter.interpret(&interpreter,statements);
/*
	    str = print(&printer,expression);
	    printf("%s\n",str);
	    free(str);
	    str = NULL;
 */
	    /* TODO need to delete expression potentially after printing */
/*	    delete_Expr(expression);*/
	    expression = NULL;
	    /*	    // For now, just print the tokens.*/
/*	    for(i = 0;i<scanner.tokens.used;i++)
	    	printf("%s\n",token_toString(&scanner.tokens.tokens[i]));
	    */
	    deleteTokenArray(&scanner.tokens);
	    init_TokenArray(&scanner.tokens);
}

 void runFile(Lox* lox,const char * file){
	FILE* inFile;
/*	char c;*/
	char* line;
	ssize_t lread;
	size_t capp;
	inFile = NULL;
	line = NULL;
	lread = capp = 0;

	if(!file){
		printf("file not given\n");
		exit( EXIT_FAILURE);
	}
	inFile = fopen(file, "r");
	if(!inFile){
		perror("Error");
		exit( EXIT_FAILURE);
	}
	while((lread = getline(&line,&capp,inFile)) > 0){
	   run(lox,line);
	   free(line);
	   line = NULL;
/*	    scanf("%c",&c);*/
	}
	 if(lox->hadError)
	   exit(65);
	if(lox->hadRuntimeError)
	    exit(70);
}

 void runPrompt(Lox* lox){
	char* line = NULL;
	size_t written = 0;
	size_t lcap = 0;
	for(;;){
		printf("> ");
		written = getline(&line,&lcap,stdin);
	    if(line){
/*		   line[written-1] = '\0';
		   if(line[written-2] == '\r')
			  line[written-2] = '\0';*/
		   run(lox,line);
		   free(line);
		   line = NULL;
		   lox->hadError = 0;
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
	if(token->type == EEOF){
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
    printf("%s\n[line %d] Runtime Error\n",e.message,e.token->line);
    lox->hadRuntimeError = 1;
}
