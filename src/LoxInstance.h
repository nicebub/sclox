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
#ifndef LCLASS_H
#define LCLASS_H
typedef struct _LoxClass LoxClass;
#endif

#ifndef LINST_H
#define LINST_H
typedef struct _LoxInstance LoxInstance;
#endif
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
