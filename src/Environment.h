/*
 * Environment.h
 *
 *  Created on: Mar 16, 2020
 *      Author: scotty
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "hash.h"
#include "HashMap.h"
#include "ReturnResult.h"
typedef struct _Environment Environment;
struct _Environment {
    Environment* Enclosing;
	HashMap *hashMap;
	void (*defineEnv)(Environment* env,char* name, ReturnResult* value);
	void* (*get)(Environment* env,Token* name);
	void (*assign)(Environment* env, Token* name, ReturnResult* value);
};

void init_Environment(Environment* env);
void init_EnvironmentwithEnclosing(Environment* env,Environment* enclosing);
Environment* create_Environment(void);
void defineEnv(Environment* env,char* name, ReturnResult *value);
void* get(Environment* env,Token* name);
void assign(Environment* env, Token* name, ReturnResult* value);
void deleteEnvironment(Environment* env);
#endif /* ENVIRONMENT_H_ */
