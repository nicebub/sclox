/*
 * Environment.c
 *
 *  Created on: Mar 16, 2020
 *      Author: scotty
 */
#define CEXCEPTION_USE_CONFIG_FILE
#include "CExceptionConfig.h"
#include "CException.h"

#include <stdio.h>
#include <assert.h>
#include "Environment.h"
#include "hash.h"
#include "StrObjHashMap.h"
#include "Token.h"
#include "str.h"
char* toStringValueDefault(void* value){
	if(value){
	    char* num,*num2 ;
	    Object * val;
	    num2 = NULL;
	    val = (Object*)value;
	    num = NULL;
	    if(val->type == NUMBER){
		   asprintf(&num,"%f",val->value.number);
		   num2 = strcopy(num2,num);
		   free(num);
		   num = NULL;
		   return num2;
	    }
	    else{
		   num = strcopy(num,val->value.string);
		   return num;
	    }
	}
	return NULL;
}

void* get(Environment* env,Token* name){
	CEXCEPTION_T e;
	StrObjHashMap* map;
	struct _Hashnode* (*get_node)(struct _HASH*,void*);
	HashMapNode* node;
	char* temp;
    temp = NULL;
	map = env->hashMap;
	get_node = map->super.super.vtable.get_node_for_key;
	if((node = (HashMapNode*)get_node((struct _HASH*)map,name->lexeme))){
		return getReference(node->super.value);
	}
    if(env->Enclosing != NULL){
	   void * temp;
	   temp = env->get(env->Enclosing,name);
	   return temp;
    }
	temp = NULL;
	temp = new_str(strlen("Undefined variable ''.")+strlen(name->lexeme));
	asprintf(&temp,"Undefined variable '%s'.",name->lexeme);
	e = create_exception(9,name,temp,NULL);
	Try{
	 Throw(e);
	}
	Catch(e){ Throw(e);}
    return NULL;
}

void defineEnv(Environment* env,char* name, Object *value){
	env->hashMap->super.super.vtable.add_to_hash((struct _HASH*)env->hashMap,name,value);
}

void init_Environment(Environment* env){
	env->hashMap = (StrObjHashMap*)create_StrObjHashMap(15);
	env->defineEnv= &defineEnv;
	env->assign = &assign;
    env->get = &get;
    env->copy = &copy_Environment;
    env->getAt = &getAt;
    env->ancestor = &ancestor;
    env->assignAt = &assignAt;
    env->Enclosing = NULL;
}
void deleteEnvironment(Environment* env){
    if(getReferenceCount(env) <= 1){
	   delete_HashMap((struct _HASH*)env->hashMap);
	   env->hashMap = NULL;
	   env->Enclosing = NULL;
	   if(getAllocated(env)){
		  delete(env);
	   }
    }
    else {
	   releaseReference(env);
    }
}

void init_EnvironmentwithEnclosing(Environment* env,Environment* enclosing){
    init_Environment(env);
    env->Enclosing = getReference(enclosing);
}
Environment* create_Environment(void){
	return NULL;
}
void assign(Environment* env, Token* name, Object* value){
	CEXCEPTION_T e;
	char * temp;
	if(env->hashMap->super.super.vtable.get_node_for_key((struct _HASH*)env->hashMap,name->lexeme)){
		env->hashMap->super.super.vtable.add_to_hash((struct _HASH*)env->hashMap,name->lexeme,value);
		return;
	}
    if(env->Enclosing != NULL){
	   env->assign(env->Enclosing,name,value);
	   return;
    }
	temp = NULL;
    temp = new_str(strlen("Undefined variable ''.")+strlen(name->lexeme));
	asprintf(&temp,"Undefined variable '%s'.",name->lexeme);
	e = create_exception(10,getReference(name),temp,NULL);
	Try{
	 Throw(e);
	}
	Catch(e){ Throw(e);}
    return;
}

void* copy_Environment(void* inE){
    Environment* out, *in;
    in = (Environment*)inE;
    if(!in) return NULL;
    out = new(OBJECTIVE,sizeof(Environment));
    init_Environment(out);
    out->Enclosing = getReference(in->Enclosing);
    out->hashMap = copy(in->hashMap);
    return out;
}
Object* getAt(Environment* env, int *distance, char* name){
	Environment* temp;
	temp =env->ancestor(env,distance);
	return temp->hashMap->super.super.vtable.get_value_for_key((struct _HASH*)temp->hashMap,name);
}
Environment* ancestor(Environment* env, int *distance){
	Environment* environ;
	int i;
	environ = env;
	for(i = 0; i < *distance;i++){
		environ = environ->Enclosing;
	}
	return environ;
}

void assignAt(Environment* env, int *distance, Token* name, Object* value){
	Environment* temp;
	temp = env->ancestor(env,distance);
	temp->hashMap->super.super.vtable.add_to_hash((struct _HASH*)temp->hashMap,name->lexeme,value);
}
