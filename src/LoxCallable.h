/*
 * LoxCallable.h
 *
 *  Created on: Mar 18, 2020
 *      Author: scotty
 */

#ifndef LOXCALLABLE_H_
#define LOXCALLABLE_H_
#include "Object.h"
#include "ObjectArray.h"
#include "Interpreter.h"
typedef struct _LoxCallable LoxCallable;
typedef struct _LoxCallable_vtable LoxCallable_vtable;
struct _LoxCallable_vtable {
	Object* (*call)(LoxCallable* inloxcall,Interpreter* intrprtr, ObjectArray* arguments);
    int (*arity)(LoxCallable* loxcall);
    char* (*toString)(LoxCallable* inloxcall);
} __attribute__((packed));
struct _LoxCallable{
    Object super;
    LoxCallable_vtable vtable;
} __attribute__((packed));

 int LoxCallablearity(LoxCallable* inloxcall);
void init_LoxCallable(LoxCallable* lcall);
char * toStringLoxCallable(LoxCallable* lcall);
Object* LoxCallablecall(LoxCallable* inloxcall,Interpreter* intrprtr, ObjectArray* arguments);
void* copy_LoxCallable(void*);
void delete_LoxCallable(void*);
#endif /* LOXCALLABLE_H_ */
