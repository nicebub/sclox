#include "LoxCallable.h"
#include <string.h>
LoxCallable_vtable lcall_vtable = {
		&LoxCallablecall,
		&LoxCallablearity,
		 &toStringLoxCallable
};
void init_LoxCallable(LoxCallable* lcall){
    mem_footer* footer;
	init_Object(&lcall->super,lcall,FUN);
/*    free(lcall->super.instanceOf);*/
    memset(&lcall->super.instanceOf,0,30);
    strncpy((char*)&lcall->super.instanceOf,"LoxCallable",strlen("LoxCallable"));
	lcall->vtable = lcall_vtable;
    footer = get_footer(lcall);
    footer->functions.allocated = 1;
    footer->functions.owner_references = 1;
    footer->functions.copy = &copy_LoxCallable;
    footer->functions.delete = &delete_LoxCallable;
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
    mem_footer* footer;
    incall = (LoxCallable*)inCall;
    lcall = new(OBJECTIVE,sizeof(LoxCallable));
    init_Object(&lcall->super,lcall,FUN);
    /*    free(lcall->super.instanceOf);*/
   memset(&lcall->super.instanceOf,0,30);
   strncpy((char*)&lcall->super.instanceOf,incall->super.instanceOf,strlen(incall->super.instanceOf));
    lcall->vtable = lcall_vtable;
    footer = get_footer(lcall);
    footer->functions.allocated = 1;
    footer->functions.owner_references = 1;
    footer->functions.copy = &copy_LoxCallable;
    footer->functions.delete = &delete_LoxCallable;
    return lcall;
}
