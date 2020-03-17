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
	HashMap *hashMap;
	void (*defineEnv)(Environment* env,char* name, ReturnResult* value);
	void* (*get)(Environment* env,Token* name);
};

void init_Environment(Environment* env);
Environment* create_Environment(void);
void defineEnv(Environment* env,char* name, ReturnResult *value);
void* get(Environment* env,Token* name);


#endif /* ENVIRONMENT_H_ */
