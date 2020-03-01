/*
 * Lox.h
 *
 *  Created on: Feb 29, 2020
 *      Author: scotty
 */

#ifndef LOX_H_
#define LOX_H_

typedef struct _Lox Lox;
typedef struct _Loxfuncs Loxfuncs;


 void init_lox(Lox* lox);

 void run(Lox* lox,char * line);

 void runFile(Lox* lox,const char * file);
 void runPrompt(Lox* lox);

 void report(Lox* lox,int line, const char * where, const char * message);
 void error(Lox* lox,int line, const char* message);

struct _Loxfuncs{
	void (*run)(Lox* lox,char*);
	void (*runPrompt)(Lox* lox);
	void (*runFile)(Lox* lox,const char*);
	void (*report)(Lox* lox,int,const char*, const char*);
	void (*error)(Lox* lox,int,const char*);

};

struct _Lox {

	int hadError;
	void (*run)(Lox* lox,char*);
	void (*runPrompt)(Lox* lox);
	void (*runFile)(Lox* lox,const char*);
	void (*report)(Lox* lox,int,const char*, const char*);
	void (*error)(Lox* lox,int,const char*);
};



#endif /* LOX_H_ */
