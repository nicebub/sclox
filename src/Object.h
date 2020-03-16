/*
 * Object.h
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "TokenType.h"

static short int ocounter=0;

typedef struct _Object Object;
struct _Object {
	short int id;
	short int owner_references;
	void* value;
	int isBool;
	TokenType type;


};

void init_Object(Object* Object, void* value, TokenType type);
void delete_Object(Object** Object);
short int getNextObjectId(void);

#endif /* OBJECT_H_ */
