/*
 * LoxInstance.h
 *
 *  Created on: Mar 31, 2020
 *      Author: scotty
 */

#ifndef LOXINSTANCE_H_
#define LOXINSTANCE_H_
#include "LoxClass.h"
#include "LoxCallable.h"
#include "StrObjHashMap.h"

typedef struct _LoxInstance LoxInstance;
struct _LoxInstance {
	LoxCallable super;
	LoxClass* klass;
	StrObjHashMap* fields;
	Object* (*get)(LoxInstance* in, Token* name);
	void (*set)(LoxInstance* in, Token* name, Object* value);
};

void init_LoxInstance(LoxInstance* in,LoxClass* cls);
char* toString_LoxInstance(LoxCallable* li);
void* copy_LoxInstance(void*);
Object* get_LoxInstance(LoxInstance* in, Token* name);
void set_LoxInstance(LoxInstance* in, Token* name, Object* value);
#endif /* LOXINSTANCE_H_ */
