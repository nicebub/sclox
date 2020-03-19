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
struct _LoxCallable{
    Object super;
	Object* (*call)(LoxCallable* inloxcall,Interpreter* intrprtr, ObjectArray* arguments);
    int (*arity)(LoxCallable* loxcall);
    char* (*toString)(LoxCallable* inloxcall);
};

#endif /* LOXCALLABLE_H_ */
