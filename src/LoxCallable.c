#include "LoxCallable.h"
#include <string.h>
LoxCallable_vtable lcall_vtable = {
		&LoxCallablecall,
		&LoxCallablearity,
		 &toStringLoxCallable
};
void init_LoxCallable(LoxCallable* lcall){
	init_Object(&lcall->super,lcall,FUN);
    setInstanceOf(&lcall->super,"LoxCallable");
/*    memset(&lcall->super.instanceOf,0,30);
    strncpy((char*)&lcall->super.instanceOf,"LoxCallable",strlen("LoxCallable"));*/
	lcall->vtable = lcall_vtable;
    setAllocated(lcall,0);
    setCopyConstructor(lcall,&copy_LoxCallable);
    setDestructor(lcall,&delete_LoxCallable);
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
void delete_LoxCallable(void* lcall){}

void* copy_LoxCallable(void* inCall){
    LoxCallable* lcall,*incall;
    incall = (LoxCallable*)inCall;
    lcall = new(OBJECTIVE,sizeof(LoxCallable));
    init_Object(&lcall->super,lcall,FUN);
    setInstanceOf(&lcall->super,incall->super.instanceOf);
/*   memset(&lcall->super.instanceOf,0,30);
   strncpy((char*)&lcall->super.instanceOf,incall->super.instanceOf,strlen(incall->super.instanceOf));*/
    lcall->vtable = lcall_vtable;
    init_LoxCallable(lcall);
    setAllocated(lcall,1);
    return lcall;
}
