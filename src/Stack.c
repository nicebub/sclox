/*
 * Stack.c
 *
 *  Created on: Mar 26, 2020
 *      Author: scotty
 */

#include "Stack.h"
#include "memory.h"
void push(Stack* stk,void* value){
	if(stk)
		stk->push(stk,value);
}
void* pop(Stack* stk){
	if(stk)
		return stk->pop(stk);
	return NULL;
}
void* top(Stack* stk){
	if(stk)
		return stk->top(stk);
	return NULL;
}
void delete_Stack(void* inStk){
	Stack* stk;
	stk = (Stack*) inStk;
	if(stk){
		if(getReferenceCount(stk) <= 1){
			while(stk->used){
				delete(pop(stk));
			}
			if(getAllocated(stk)){
				delete(stk);
				stk = NULL;
			}
		}
		else{
			releaseReference(stk);
		}
	}
}
void* init_Stack(void* inStk, void* arguments){
	Stack* stk;
	stk = (Stack*)inStk;
	stk->values = NULL;
	stk->size = 0;
	stk->used = 0;
	stk->push = &push;
	stk->pop = &pop;
	stk->top = &top;
	setCopyConstructor(stk,&copy_Stack);
	setDestructor(stk,&delete_Stack);
	setConstructor(stk,&init_Stack);
	return stk;

}
void* copy_Stack(void* inStk){
	Stack* stk, *newstk;
	stk = (Stack*) inStk;
	newstk = new(OBJECTIVE,sizeof(Stack));
	init_Stack(newstk,NULL);
	newstk->size = stk->size;
	newstk->used = stk->used;
	newstk->values = copy(stk->values);
	setAllocated(newstk,1);
	return newstk;
}
