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
	Object* (*call)(Interpreter* intrprtr, ObjectArray* arguments);
};

#endif /* LOXCALLABLE_H_ */
