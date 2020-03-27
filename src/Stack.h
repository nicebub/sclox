/*
 * Stack.h
 *
 *  Created on: Mar 26, 2020
 *      Author: scotty
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct _Stack Stack;
struct _Stack {
	void** values;
	int size;
	int used;
	void (*push)(Stack*,void*);
	void* (*pop)(Stack*);
	void* (*top)(Stack*);
};

void push(Stack* stk,void* value);
void* pop(Stack* stk);
void* top(Stack* stk);
void delete_Stack(void* stk);
void* init_Stack(void* stk,void * arguments);
void* copy_Stack(void* stk);
#endif /* STACK_H_ */
