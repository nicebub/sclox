/*
 * Object.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "TokenType.h"
 struct _LoxCallable;

static short int ocounter=0;

typedef union _ResultValue ResultValue;
typedef struct _Object Object;
union _ResultValue {
	double number;
	char* string;
	struct _LoxCallable* callable;
};
struct _Object {
	short int id;
	short int owner_references;
	ResultValue value;
	int isBool;
	TokenType type;
    char instanceOf[30];


};
Object* copyObject(Object* inobj);
Object* releaseObjectReference(Object* obj);
void init_Object(Object* Object, void* value, TokenType type);
void delete_Object(Object** Object);
short int getNextObjectId(void);
Object* getObjectReference(Object* literal);
#endif /* OBJECT_H_ */
