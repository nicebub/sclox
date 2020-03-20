#include "LoxCallable.h"
#include <string.h>
LoxCallable_vtable lcall_vtable = {
		&LoxCallablecall,
		&LoxCallablearity,
		 &toStringLoxCallable
};
void init_LoxCallable(LoxCallable* lcall){
	init_Object(&lcall->super,lcall,FUN);
/*    free(lcall->super.instanceOf);*/
    memset(&lcall->super.instanceOf,0,30);
    strcpy((char*)&lcall->super.instanceOf,"LoxCallable");
	lcall->vtable = lcall_vtable;
}


 int LoxCallablearity(LoxCallable* inloxcall){
	return inloxcall->vtable.arity(inloxcall);
}
char * toStringLoxCallable(LoxCallable* lcall){
	return lcall->vtable.toString(lcall);
}
Object* LoxCallablecall(LoxCallable* inloxcall,Interpreter* intrprtr, ObjectArray* arguments){
	return inloxcall->vtable.call(inloxcall,intrprtr,arguments);
}
