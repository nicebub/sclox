/*
 * LoxInstance.c
 *
 *  Created on: Mar 31, 2020
 *      Author: scotty
 */
#define CEXCEPTION_USE_CONFIG_FILE
#include "CExceptionConfig.h"
#include "CException.h"
#include "HashMap.h"
#include "StrObjHashMap.h"
#include "LoxInstance.h"
#include "LoxClass.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "str.h"
void init_LoxInstance(LoxInstance* in,LoxClass* kls){
	init_LoxCallable(&in->super);
	in->super.vtable.toString = &toString_LoxInstance;
	in->klass = kls;
    setInstanceOf(&in->super.super,"LoxInstance");
/*    memset(&in->super.super.instanceOf,0,30);
    strncpy((char*)&in->super.super.instanceOf,"LoxInstance",strlen("LoxInstance")+1);*/
    setCopyConstructor(in,&copy_LoxInstance);
    in->get = &get_LoxInstance;
    in->set = &set_LoxInstance;
    in->fields = create_StrObjHashMap(40);
}
char* toString_LoxInstance(LoxCallable* li){
	char* temp,*other;
    temp = new_str(strlen(((LoxInstance*)li)->klass->name) + strlen("instance")+1);
    other = NULL;
	asprintf(&other,"%s instance",((LoxInstance*)li)->klass->name);
    strcpy(temp,other);
    free(other);
	return temp;
}

void* copy_LoxInstance(void* inIn){
    LoxInstance* instance,*nin;
    instance = (LoxInstance*)inIn;
    nin = new(OBJECTIVE,sizeof(LoxInstance));
    init_LoxInstance(nin,instance->klass);
    nin->super.super.id = instance->super.super.id;
    nin->fields = getReference(instance->fields);
    return nin;
}


Object* get_LoxInstance(LoxInstance* inst, Token* name){
	CEXCEPTION_T e;
    char* temp_str;
	LoxFunction* method;
	if(inst->fields->super.super.vtable.get_value_for_key((struct _HASH*)inst->fields,name->lexeme)){
		return inst->fields->super.super.vtable.get_value_for_key((struct _HASH*)inst->fields,name->lexeme);
	}
	method = inst->klass->findMethod((LoxFunction*)inst->klass,name->lexeme);
	if(method != NULL)
		return (Object*)method->bind(method,inst);
    temp_str = new_str((strlen("Undefined property ''.")+strlen(name->lexeme)));
    asprintf(&temp_str,"Undefined property '%s'.",name->lexeme);
    e = create_exception(55,name,temp_str,NULL);
	Try{
	 Throw(e);
	}
	Catch(e){ Throw(e);}
	return NULL;
}

void set_LoxInstance(LoxInstance* inst, Token* name, Object* value){
	inst->fields->super.super.vtable.add_to_hash((struct _HASH*)inst->fields,name->lexeme,value);
}
