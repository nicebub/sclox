/*
 * Environment.h
 *
 *  Created on: Mar 16, 2020
 *      Author: scotty
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "hash.h"
#include "StrObjHashMap.h"
#include "Object.h"
typedef struct _Environment Environment;
struct _Environment {
    Environment* Enclosing;
	StrObjHashMap *hashMap;
	void (*defineEnv)(Environment* env,char* name, Object* value);
	void* (*get)(Environment* env,Token* name);
	void (*assign)(Environment* env, Token* name, Object* value);
    void* (*copy)(void* environment);
    Object* (*getAt)(Environment* env, int *distance, char* name);
    Environment* (*ancestor)(Environment* env, int *distance);
    void (*assignAt)(Environment* env, int *distance, Token* name, Object* value);
};

void init_Environment(Environment* env);
void init_EnvironmentwithEnclosing(Environment* env,Environment* enclosing);
Environment* create_Environment(void);
void defineEnv(Environment* env,char* name, Object *value);
void* get(Environment* env,Token* name);
void assign(Environment* env, Token* name, Object* value);
void deleteEnvironment(Environment* env);
void* copy_Environment(void* in);
Object* getAt(Environment* env, int* distance, char* name);
Environment* ancestor(Environment* env, int* distance);
void assignAt(Environment* env, int* distance, Token* name, Object* value);
#endif /* ENVIRONMENT_H_ */
