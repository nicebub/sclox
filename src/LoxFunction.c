/*
 * LoxFunction.c
 *
 *  Created on: Mar 19, 2020
 *      Author: scotty
 */

#include "LoxFunction.h"
#include "LoxCallable.h"
#include <string.h>
LoxFunction_vtable lfunc_vtable = {
		&LoxFunctioncall,
		&LoxFunctionarity,
		&toStringLoxFunction
};

void init_LoxFunction(LoxFunction* func, Function* declaration) {
	init_LoxCallable(&func->super);
    func->super.vtable = lfunc_vtable;
    memset(&func->super.super.instanceOf,0,30);
    strcpy((char*)&func->super.super.instanceOf,"LoxCallable");
	func->declaration = declaration;
}

Object* LoxFunctioncall(LoxCallable* lfunc, Interpreter* intprtr, ObjectArray* arguments){
	Environment* env;
	LoxFunction* function;
	TokenArray* params;
	int i;
	function = (LoxFunction*)lfunc;
	env = malloc(sizeof(Environment));
	init_EnvironmentwithEnclosing(env,intprtr->globals);
	params = function->declaration->params;
	for(i=0; i< function->declaration->params->used ; i++){
		env->defineEnv(env,params->getElementInArrayAt(params,i)->lexeme,arguments->getElementInArrayAt(arguments,i));
	}
	intprtr->executeBlock(intprtr,function->declaration->body,env);
	return NULL;
}

int LoxFunctionarity(LoxCallable* loxcall){
	return ((LoxFunction*)loxcall)->declaration->params->used;
}
char* toStringLoxFunction(LoxCallable* inloxcall){
	char * new_str;
	new_str=NULL;
	asprintf(&new_str,"<fn %s>",((LoxFunction*)inloxcall)->declaration->name->lexeme);
	return new_str;
}
