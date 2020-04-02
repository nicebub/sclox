/*
 * LoxClass.c
 *
 *  Created on: Mar 31, 2020
 *      Author: scotty
 */

#include "LoxClass.h"
#include "LoxInstance.h"
void init_LoxClass(LoxClass* cl,char* name){
    init_LoxCallable(&cl->super);
    cl->super.vtable.toString = &toString_LoxClass;
    cl->super.vtable.arity = &arity_LoxClass;
    cl->super.vtable.call = &call_LoxClass;
    cl->findMethod = &findMethod;
    setCopyConstructor(cl,&copy_LoxClass);
    cl->name = name;
    memset(&cl->super.super.instanceOf,0,30);
    strncpy((char*)&cl->super.super.instanceOf,"LoxClass",strlen("LoxClass")+1);
}
void init_LoxClassWithMethods(LoxClass* cl,char* name,StrObjHashMap* methods){
	init_LoxClass(cl,name);
	cl->methods = methods;
}

char* toString_LoxClass(LoxCallable* cl){
    LoxClass* class;
    class = (LoxClass*) cl;
    if(class)
	   return getReference(class->name);
    return NULL;
}

Object* call_LoxClass(LoxCallable* lc,Interpreter* interpreter, ObjectArray* arguments){
    LoxInstance* instance;
    instance = new(OBJECTIVE,sizeof(LoxInstance));
    init_LoxInstance(instance,(LoxClass*)lc);
    return (Object*)instance;
}

int arity_LoxClass(LoxCallable* lc){
    return 0;
}

void* copy_LoxClass(void* inCls){
    LoxClass* cls, *in;
    in = (LoxClass*) inCls;
    cls = new(OBJECTIVE,sizeof(LoxClass));
    init_LoxClass(cls,in->name);
    cls->super.super.id = in->super.super.id;
    return cls;
}
LoxFunction* findMethod(LoxFunction* func, char* name){
	if(((LoxClass*)func)->methods->super.super.vtable.get_value_for_key((struct _HASH*)((LoxClass*)func)->methods,name)){
		return ((LoxClass*)func)->methods->super.super.vtable.get_value_for_key((struct _HASH*)((LoxClass*)func)->methods,name);
	}
	return NULL;
}
