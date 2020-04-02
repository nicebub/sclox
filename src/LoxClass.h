/*
 * LoxClass.h
 *
 *  Created on: Mar 31, 2020
 *      Author: scotty
 */

#ifndef LOXCLASS_H_
#define LOXCLASS_H_

#include "LoxCallable.h"
#include "StrObjHashMap.h"
#include "LoxFunction.h"
#ifndef LFUNC_H
#define LFUNC_H
typedef struct _LoxFunction LoxFunction;
#endif
#ifndef LCLASS_H
#define LCLASS_H
typedef struct _LoxClass LoxClass;
#endif
struct _LoxClass {
	LoxCallable super;
    char* name;
    StrObjHashMap* methods;
    LoxFunction* (*findMethod)(LoxFunction* func, char* name);
};

void init_LoxClass(LoxClass* cl,char* name);
void init_LoxClassWithMethods(LoxClass* cl,char* name,StrObjHashMap* methods);
char* toString_LoxClass(LoxCallable* cl);
Object* call_LoxClass(LoxCallable* lc,Interpreter* interpreter, ObjectArray* arguments);
int arity_LoxClass(LoxCallable* lc);
void* copy_LoxClass(void*);
LoxFunction* findMethod(LoxFunction* func, char* name);
#endif /* LOXCLASS_H_ */
