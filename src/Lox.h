/*
 * Lox.h
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#ifndef LOX_H_
#define LOX_H_
#define CEXCEPTION_USE_CONFIG_FILE
#include "CException.h"
#include "Token.h"
#include "Interpreter.h"
#include "Scanner.h"
#include "Parser.h"
#ifndef _LOX
#define _LOX
typedef struct _Lox Lox;
#endif
typedef struct _Loxfuncs Loxfuncs;


 void init_lox(Lox* lox);

 void run(Lox* lox,char * line);

 void runFile(Lox* lox,const char * file);
 void runPrompt(Lox* lox);

 void report(Lox* lox,int line, const char * where, const char * message);
 /*static void error(Lox* lox,int line, const char* message);*/

struct _Loxfuncs{
	void (*run)(Lox* lox,char*);
	void (*runPrompt)(Lox* lox);
	void (*runFile)(Lox* lox,const char*);
	void (*report)(Lox* lox,int,const char*, const char*);
	void (*error)(Lox* lox,int,const char*);
	void (*parse_error)(Lox* lox,Token* token, const char* message);
     void (*runtimeError)(Lox* lox, CEXCEPTION_T e);

};

struct _Lox {

	short int hadError;
    short int hadRuntimeError;
	Interpreter interpreter;
	/*	    AstPrinter printer;*/
	Scanner scanner;
	Parser parser;
	StmtArray* statements;
	void (*run)(Lox* lox,char*);
	void (*runPrompt)(Lox* lox);
	void (*runFile)(Lox* lox,const char*);
	void (*report)(Lox* lox,int,const char*, const char*);
	void (*error)(Lox* lox,int,const char*);
	void (*parse_error)(Lox* lox,Token* token, const char* message);
     void (*runtimeError)(Lox* lox, CEXCEPTION_T e);};



#endif /* LOX_H_ */
