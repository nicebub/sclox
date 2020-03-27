/*
 * HashMapStack.h
 *
 *  Created on: Mar 26, 2020
 *      Author: scotty
 */

#ifndef HASHMAPSTACK_H_
#define HASHMAPSTACK_H_
#include "HashMap.h"
#include "Stack.h"
typedef struct _HashMapStack HashMapStack;
struct _HashMapStack {
	Stack super;
};
void pushHashMapStack(Stack* stk,void* value);
void* popHashMapStack(Stack* stk);
void* topHashMapStack(Stack* stk);
void delete_HashMapStack(void* stk);
void* init_HashMapStack(void* stk,void * arguments);
void* copy_HashMapStack(void* stk);

#endif /* HASHMAPSTACK_H_ */
