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
		num = NULL;
		asprintf(&num,"%d",*(int*)value);
		return num;
	}
	return NULL;
}

void* get(Environment* env,Token* name){
	CEXCEPTION_T e;
	HashMap* map;
	struct _Hashnode* (*get_node)(struct _HASH*,void*);
	HashMapNode* node;
	char* temp;
	map = env->hashMap;
	get_node = env->hashMap->super.vtable.get_node_for_key;
	if((node = (HashMapNode*)get_node((struct _HASH*)map,name->lexeme))){
		return node->super.value;
	}
	return NULL;
	temp = NULL;
	asprintf(&temp,"Undefined variable '%s'.",name->lexeme);
	e.token = name;
	e.message = temp;
	Throw(e);
/*	Throw(runtimeError(name,);*/
}

void defineEnv(Environment* env,char* name, ReturnResult *value){
	env->hashMap->super.vtable.add_to_hash((struct _HASH*)env->hashMap,name,(void*)value);
}

void init_Environment(Environment* env){
	env->hashMap = (HashMap*)create_hashmap(15,&func);
	env->defineEnv= defineEnv;
    env->get = &get;
}
Environment* create_Environment(void){
	return NULL;
}
