/*
 * LoxFunction.c
 *
 *  Created on: Mar 19, 2020
 *      Author: scotty
 */
#define CEXCEPTION_USE_CONFIG_FILE
#include "CExceptionConfig.h"
#include "CException.h"
#include "LoxFunction.h"
#include "LoxCallable.h"
#include "LoxInstance.h"
#include "Environment.h"
#include <string.h>
#include "str.h"
LoxFunction_vtable lfunc_vtable = {
		&LoxFunctioncall,
		&LoxFunctionarity,
		&toStringLoxFunction
};

void init_LoxFunction(LoxFunction* func, Function* declaration) {
	init_LoxCallable(&func->super);
    func->super.vtable = lfunc_vtable;
    setInstanceOf(&func->super.super,"LoxFunction");
/*    memset(&func->super.super.instanceOf,0,30);
    strncpy((char*)&func->super.super.instanceOf,"LoxFunction",strlen("LoxFunction")+1);*/
	func->declaration = declaration;
    func->closure = NULL;
    func->bind = &bind_LoxFunction;
    setAllocated(func,1);
    setCopyConstructor(func,&copy_LoxFunction);
    setDestructor(func,&delete_LoxFunction);
    func->isInitializer =0;
}
void init_LoxFunctionWithClosure(LoxFunction* func, Function* declaration, Environment* closure){
    init_LoxFunction(func,declaration);
    func->closure = getReference(closure);
}
void init_LoxFunctionWithClosureAndInitializer(LoxFunction* func, Function* declaration, Environment* closure, short int initializer){
    init_LoxFunctionWithClosure(func,declaration,closure);
    func->isInitializer = initializer;
}

Object* LoxFunctioncall(LoxCallable* lfunc, Interpreter* intprtr, ObjectArray* arguments){
	CEXCEPTION_T re;
    Object* result;
    Environment* env;
    LoxFunction* function;
    TokenArray* params;
    int i;
    re = create_exception(0,NULL,NULL,NULL);
    result = NULL;
	function = (LoxFunction*)lfunc;
	env = new(OBJECTIVE,sizeof(Environment));
	init_EnvironmentwithEnclosing(env,function->closure);
	params = function->declaration->params;
	for(i=0; i< function->declaration->params->used ; i++){
		env->defineEnv(env,(params->getElementInArrayAt(params,i)->lexeme),copy(arguments->getElementInArrayAt(arguments,i)));
	}
	Try{
	intprtr->executeBlock(intprtr,function->declaration->body,env);
	}
	Catch(re){
/*	    if(function->isInitializer){
		   char* str;
		   int *x;
		   str = NULL;
		   str = strcopy(str,"this");
		   x = new(RAW,sizeof(int));
		   *x = 0;
		   return function->closure->getAt(function->closure,x,str);
	    }*/
	    if(re.sub)
		   result = re.sub->value;
	    else{
		   CEXCEPTION_T e;
		  Try{
		   Throw(e);
		  }
		  Catch(e){ Throw(e);}
	    }
	}
    if(re.sub)
	   result = re.sub->value;
    if(function->isInitializer){
	   char * str;
	   int *x;
	   str = NULL;
	   str = strcopy(str,"this");
	   x = new(RAW,sizeof(int));
	   *x = 0;
	   return function->closure->getAt(function->closure,x,str);
    }
    return result;
}

int LoxFunctionarity(LoxCallable* loxcall){
	return ((LoxFunction*)loxcall)->declaration->params->used;
}
char* toStringLoxFunction(LoxCallable* inloxcall){
	char * anew_str,*temp;
	anew_str=NULL;
    temp = NULL;
    anew_str = new_str(strlen("<fn >")+strlen(((LoxFunction*)inloxcall)->declaration->name->lexeme));
	asprintf(&temp,"<fn %s>",((LoxFunction*)inloxcall)->declaration->name->lexeme);
    strncpy(anew_str,temp,strlen("<fn >")+strlen(((LoxFunction*)inloxcall)->declaration->name->lexeme));
    free(temp);
    temp = NULL;
	return anew_str;
}
void* copy_LoxFunction(void* inFunc){
    LoxFunction* func, *infunc;
    infunc = (LoxFunction*)inFunc;
    func = new(OBJECTIVE,sizeof(LoxFunction));
	init_LoxCallable(&func->super);
    func->super.vtable = lfunc_vtable;
    setInstanceOf(&func->super.super,"LoxFunction");
/*    memset(&func->super.super.instanceOf,0,30);
    strncpy((char*)&func->super.super.instanceOf,"LoxFunction",strlen("LoxFunction")+1);*/
    init_LoxFunctionWithClosure(func,copy(infunc->declaration),infunc->closure);
    setAllocated(func,1);
    return func;

}
void delete_LoxFunction(void* inFunc){
    
}
LoxFunction* bind_LoxFunction(LoxFunction* func, LoxInstance* instance){
	Environment* env;
    char* str;
	LoxFunction* function;
	env = new(OBJECTIVE, sizeof(Environment));
	init_EnvironmentwithEnclosing(env,func->closure);
    str = NULL;
    str = strcopy(str,"this");
	env->defineEnv(env,str,(Object*)instance);
	function = new(OBJECTIVE,sizeof(LoxFunction));
	init_LoxFunctionWithClosureAndInitializer(function,func->declaration,env,func->isInitializer);
	return function;
}
