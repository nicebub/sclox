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

typedef struct _LoxFunction LoxFunction;
typedef struct _LoxCallable_vtable LoxFunction_vtable;
struct _LoxFunction {
	LoxCallable super;
	Function* declaration;
    Environment* closure;

} __attribute__((packed));
Object* LoxFunctioncall(LoxCallable* lfunc, Interpreter* intprtr, ObjectArray* arguments);
int LoxFunctionarity(LoxCallable* loxcall);
char*toStringLoxFunction(LoxCallable* inloxcall);

void init_LoxFunction(LoxFunction* func, Function* declaration);
void init_LoxFunctionWithClosure(LoxFunction* func, Function* declaration, Environment* closure);
void* copy_LoxFunction(void*);
void delete_LoxFunction(void*);
#endif /* LOXFUNCTION_H_ */
