/*
 * Environment.c
 *
 *  Created on: Mar 16, 2020
 *      Author: scotty
 */
#include "CExceptionConfig.h"
#include "CException.h"

#include <stdio.h>
#include "Environment.h"
#include "hash.h"
#include "HashMap.h"
#include "Token.h"
char* func(void* value){
	if(value){
	    char* num ;
	    Object * val = (Object*)value;
	    if(val->type == NUMBER){
		   num = NULL;
		   asprintf(&num,"%f",val->value.number);
		   return num;
	    }
	    else{
		   num = NULL;
		   return strdup(val->value.string);
	    }
	}
	return NULL;
}

void* get(Environment* env,Token* name){
	CEXCEPTION_T e;
	HashMap* map;
	struct _Hashnode* (*get_node)(struct _HASH*,void*);
	HashMapNode* node;
	char* temp;
    temp = NULL;
	map = env->hashMap;
	get_node = env->hashMap->super.vtable.get_node_for_key;
	if((node = (HashMapNode*)get_node((struct _HASH*)map,name->lexeme))){
		return node->super.value;
	}
    if(env->Enclosing != NULL){
	   void * temp;
	   temp = env->get(env->Enclosing,name);
	   return temp;
    }
	temp = NULL;
	asprintf(&temp,"Undefined variable '%s'.",name->lexeme);
    e.id = 9;
	e.token = name;
	e.message = temp;
	Throw(e);
    return NULL;
/*	Throw(runtimeError(name,);*/
}

void defineEnv(Environment* env,char* name, Object *value){
	env->hashMap->super.vtable.add_to_hash((struct _HASH*)env->hashMap,name,value);
}

void init_Environment(Environment* env){
	env->hashMap = (HashMap*)create_hashmap(15,&func);
	env->defineEnv= &defineEnv;
	env->assign = &assign;
    env->get = &get;
    env->Enclosing = NULL;
}
void deleteEnvironment(Environment* env){
    delete_hashmap((struct _HASH*)env->hashMap);
    env->hashMap = NULL;
    env->Enclosing = NULL;
}

void init_EnvironmentwithEnclosing(Environment* env,Environment* enclosing){
    init_Environment(env);
    env->Enclosing = enclosing;
}
Environment* create_Environment(void){
	return NULL;
}
void assign(Environment* env, Token* name, Object* value){
	CEXCEPTION_T e;
	char * temp;
	if(env->hashMap->super.vtable.get_node_for_key((struct _HASH*)env->hashMap,name->lexeme)){
		env->hashMap->super.vtable.add_to_hash((struct _HASH*)env->hashMap,name->lexeme,value);
		return;
	}
    if(env->Enclosing != NULL){
	   env->assign(env->Enclosing,name,value);
	   return;
    }
	temp = NULL;
	asprintf(&temp,"Undefined variable '%s'.",name->lexeme);
    e.id = 10;
	e.token = name;
	e.message = temp;
	Throw(e);
    return;
}
