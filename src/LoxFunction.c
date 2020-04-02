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
    strncpy((char*)&func->super.super.instanceOf,"LoxFunction",strlen("LoxFunction")+1);
	func->declaration = declaration;
    func->closure = NULL;
    setAllocated(func,1);
    setCopyConstructor(func,&copy_LoxFunction);
    setDestructor(func,&delete_LoxFunction);
}
void init_LoxFunctionWithClosure(LoxFunction* func, Function* declaration, Environment* closure){
    init_LoxFunction(func,declaration);
    func->closure = getReference(closure);
}

Object* LoxFunctioncall(LoxCallable* lfunc, Interpreter* intprtr, ObjectArray* arguments){
	CEXCEPTION_T re;
    Object* result;
    Environment* env;
    LoxFunction* function;
    TokenArray* params;
    int i;
    re.id = 0;
    re.message = NULL;
    re.sub = NULL;
    re.token = NULL;
    result = NULL;
	function = (LoxFunction*)lfunc;
	env = new(OBJECTIVE,sizeof(Environment));
/*	init_EnvironmentwithEnclosing(env,intprtr->globals);*/
	init_EnvironmentwithEnclosing(env,function->closure);
	params = function->declaration->params;
	for(i=0; i< function->declaration->params->used ; i++){
		env->defineEnv(env,(params->getElementInArrayAt(params,i)->lexeme),copy(arguments->getElementInArrayAt(arguments,i)));
	}
	Try{
	intprtr->executeBlock(intprtr,function->declaration->body,env);
	}
	Catch(re){
	    if(re.sub)
		   result = re.sub->value;
	    else
		  Throw(re);
	}
    if(re.sub)
	   result = re.sub->value;
	   return result;
}

int LoxFunctionarity(LoxCallable* loxcall){
	return ((LoxFunction*)loxcall)->declaration->params->used;
}
char* toStringLoxFunction(LoxCallable* inloxcall){
	char * new_str,*temp;
	new_str=NULL;
    temp = NULL;
    new_str = new(RAW,sizeof(char)*(strlen("<fn >")+strlen(((LoxFunction*)inloxcall)->declaration->name->lexeme)+1));
    memset(new_str,0,1+strlen("<fn >")+strlen(((LoxFunction*)inloxcall)->declaration->name->lexeme));
	asprintf(&temp,"<fn %s>",((LoxFunction*)inloxcall)->declaration->name->lexeme);
    strncpy(new_str,temp,strlen("<fn >")+strlen(((LoxFunction*)inloxcall)->declaration->name->lexeme));
    free(temp);
    temp = NULL;
	return new_str;
}
void* copy_LoxFunction(void* inFunc){
    LoxFunction* func, *infunc;
    infunc = (LoxFunction*)inFunc;
    func = new(OBJECTIVE,sizeof(LoxFunction));
	init_LoxCallable(&func->super);
    func->super.vtable = lfunc_vtable;
    memset(&func->super.super.instanceOf,0,30);
    strncpy((char*)&func->super.super.instanceOf,"LoxFunction",strlen("LoxFunction")+1);
    init_LoxFunctionWithClosure(func,copy(infunc->declaration),infunc->closure);
    setAllocated(func,1);
    return func;

}
void delete_LoxFunction(void* inFunc){
    
}
