/*
 * Object.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "memory.h"
#include "TokenType.h"
 struct _LoxCallable;

static short int ocounter=0;

typedef union _ResultValue ResultValue;
typedef struct _Object Object;
union _ResultValue {
	double number;
	char* string;
	struct _LoxCallable* callable;
}__attribute__((packed));
typedef struct _Object_vtable Object_vtable;

struct _Object_vtable {
	void* (*copy)(void* inobj);
	void (*init)(Object* Object, void* value, TokenType type);
	void (*delete)(void* Object);
	short int (*getNextId)(void);
}__attribute__((packed));
struct _Object {
	Object_vtable vtable;
	short int id;
	ResultValue value;
	int isBool;
	TokenType type;
    char instanceOf[30];


}__attribute__((packed));
void* copy_Object(void* inobj);
void init_Object(Object* Object, void* value, TokenType type);
void delete_Object(void* Object);
short int getNextObjectId(void);
#endif /* OBJECT_H_ */
