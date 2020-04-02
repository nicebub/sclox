/*
 * LoxFunction.h
 *
 *  Created on: Mar 19, 2020
 *      Author: scotty
 */

#ifndef LOXFUNCTION_H_
#define LOXFUNCTION_H_
#include "LoxCallable.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxInstance.h"
#ifndef LFUNC_H
#define LFUNC_H
typedef struct _LoxFunction LoxFunction;
#endif
#ifndef LINST_H
#define LINST_H
typedef struct _LoxInstance LoxInstance;
#endif
typedef struct _LoxCallable_vtable LoxFunction_vtable;
struct _LoxFunction {
	LoxCallable super;
	Function* declaration;
    Environment* closure;
    short int isInitializer;
	LoxFunction* (*bind)(LoxFunction* func,LoxInstance* this);
} __attribute__((packed));
Object* LoxFunctioncall(LoxCallable* lfunc, Interpreter* intprtr, ObjectArray* arguments);
int LoxFunctionarity(LoxCallable* loxcall);
char*toStringLoxFunction(LoxCallable* inloxcall);
LoxFunction* bind_LoxFunction(LoxFunction* func, LoxInstance* this);

void init_LoxFunction(LoxFunction* func, Function* declaration);
void init_LoxFunctionWithClosure(LoxFunction* func, Function* declaration, Environment* closure);
void init_LoxFunctionWithClosureAndInitializer(LoxFunction*, Function* decl, Environment* closure, short int isInitializer);
void* copy_LoxFunction(void*);
void delete_LoxFunction(void*);
#endif /* LOXFUNCTION_H_ */
