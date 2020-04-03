/*
 * LoxClass.c
 *
 *  Created on: Mar 31, 2020
 *      Author: scotty
 */

#include "LoxClass.h"
#include "LoxInstance.h"
#include "str.h"
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
void init_LoxClassWithMethodsAndSuper(LoxClass* cl,char* name,LoxClass* superclass,StrObjHashMap* methods){
	init_LoxClassWithMethods(cl,name,methods);
	cl->superclass = superclass;
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
    LoxFunction* initializer;
    char* str;
    instance = new(OBJECTIVE,sizeof(LoxInstance));
    init_LoxInstance(instance,(LoxClass*)lc);
    str = NULL;
    str = strcopy(str,"init");
/*    str = new(RAW,sizeof(char)*(strlen("init")+1));
    memset(str,0,strlen("init")+1);
    strcpy(str,"init");*/
    initializer = findMethod((LoxFunction*)lc,str);
    if(initializer != NULL){
	   LoxCallable* temp;
	   temp = ((LoxCallable*)initializer->bind(initializer,instance));
	   temp->vtable.call(temp,interpreter,arguments);
    }
    return (Object*)instance;
}

int arity_LoxClass(LoxCallable* lc){
    LoxFunction * initializer;
    char* str;
    str = NULL;
    str = strcopy(str,"init");
/*    str = new(RAW,sizeof(char)*(strlen("init")+1));
    memset(str,0,strlen("init")+1);
    strcpy(str,"init");*/
    initializer = findMethod((LoxFunction*)lc,str);
    if(initializer == NULL) return 0;
    return ((LoxFunction*)initializer)->super.vtable.arity((LoxCallable*)initializer);
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
	if(((LoxClass*)func)->superclass != NULL){
		return ((LoxClass*)func)->superclass->findMethod((LoxFunction*)((LoxClass*)func)->superclass,name);
	}
	return NULL;
}
