/*
 * HashMapStack.c
 *
 *  Created on: Mar 26, 2020
 *      Author: scotty
 */

#include "HashMapStack.h"
#include "Stack.h"
#include "memory.h"

void* init_HashMapStack(void* inStk,void * arguments){
	HashMapStack* stk;
	size_t size;
/*	HashMap* map;*/
	int i;
	size = *(size_t*)arguments;
	stk = (HashMapStack*) inStk;
	init_Stack(&stk->super,arguments);
	stk->super.values = new(OBJECTIVE,sizeof(HashMap*)*(size));
			for(i=0;i<size;i++){
				((HashMap**)stk->super.values)[i] = NULL;
			}
	stk->super.top = &topHashMapStack;
	stk->super.pop = &popHashMapStack;
	stk->super.push = &pushHashMapStack;
	setCopyConstructor(stk,&copy_HashMapStack);
	setConstructor(stk,&init_Stack);
	setDestructor(stk,&delete_HashMapStack);
	return stk;
}


void pushHashMapStack(Stack* inStk,void* value){
	HashMapStack* stk;
	stk = (HashMapStack*)inStk;
	if(stk){
		if(stk->super.size > stk->super.used){
			stk->super.values[stk->super.used] =getReference(value);
			stk->super.used++;
		}
	}
}
void* popHashMapStack(Stack* inStk){
	HashMapStack *stk,*temp;
	stk = (HashMapStack*)inStk;
	if(stk){
		if(stk->super.used > 0){
			temp = getReference(((HashMap**)stk->super.values)[stk->super.used-1]);
			releaseReference(((HashMap**)stk->super.values)[stk->super.used-1]);
			((HashMap**)stk->super.values)[stk->super.used-1] = NULL;
			stk->super.used--;
			return temp;
		}
	}
	return NULL;

}
void* topHashMapStack(Stack* inStk){
	HashMapStack * stk;
	stk = (HashMapStack*) inStk;
	if(stk){
		return ((HashMap**)stk->super.values)[stk->super.used-1];
	}
	return NULL;
}
void delete_HashMapStack(void* stk){
	delete_Stack(stk);
}
void* copy_HashMapStack(void* inStk){
	HashMapStack* stk;
	stk = (HashMapStack*) inStk;
	if(stk){
		int i;
		HashMapStack* cpy,*stk;
		stk = (HashMapStack*) inStk;
		cpy = new(OBJECTIVE,sizeof(HashMapStack));
		init_HashMapStack(cpy,&stk->super.size);
		cpy->super.size = stk->super.size;
		cpy->super.used = stk->super.used;
		for(i=0;i<stk->super.used;i++){
			cpy->super.values[i] = copy(stk->super.values[i]);
		}
		setAllocated(cpy,1);
		return cpy;
	}
	return NULL;
}
