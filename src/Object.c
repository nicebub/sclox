/*
 * Object.c
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#include <string.h>
#include <stdio.h>
#include "Object.h"
#include "TokenType.h"
#include "LoxCallable.h"
#include "LoxFunction.h"
#include "str.h"
mem_funcs memfuncs = {
		&delete_Object,
		NULL,
	   NULL, /* init */
		&copy_Object,
		&resize,
    &getAllocated,
    &setAllocated,
    &setCopyConstructor,
    &setDestructor,
    &setConstructor,
    &get_header,
    &get_footer
};

Object_vtable ovtable = {
		&copy_Object,
		&init_Object,
		delete_Object,
		&getNextObjectId,

};
void* new_Object(void* value, TokenType type){
	Object* obj;
	obj = new(OBJECTIVE,sizeof(Object));
	init_Object(obj,value,type);
    setAllocated(obj,1);
	return obj;
}
void init_Object(Object* object, void* value, TokenType type){
	if(!object) return;
    object->value.string = NULL;
    
	object->id = getNextObjectId();
	if(value){

		switch(type){
		    case KNULL:
			   break;
			case NUMBER:
				object->value.number = *(double*)value;
				break;
			case FUN:
			case CLASS:
			   object->value.string = NULL;
				break;
			case STRING:
			default:
			   object->value.string = strcopy(object->value.string,value);
				break;
		}
	}
	else object->value.string = NULL;
	object->type = type;
	object->isBool = type==FALSE||type==TRUE||type==NIL?1:0;
    setInstanceOf(object,"Object");
/*    memset((char*)&object->instanceOf,0,30);
    strncpy((char*)&object->instanceOf,"Object",strlen("Object"));*/
    object->vtable = ovtable;
    setCopyConstructor(object,&copy_Object);
    setDestructor(object,&delete_Object);
}

void delete_Object(void* inObject){
	Object* object;
	object = (Object*)inObject;
	if(object){
		if(getReferenceCount(object) <= 1){
				switch((object)->type){
				case NUMBER:
					break;
				case FUN:
				case CLASS:
					break;
				case STRING:
				default:
					   if((object)->value.string){
						(object)->value.string = NULL;
					}
					break;
				}
			object = NULL;
			object = NULL;
		}
		else{
			releaseReference(object);
		}
	}
}
short int getNextObjectId(void){
	ocounter += 1;
	return ocounter;
}

void* copy_Object(void* ninobj){
	Object* newobj,*inobj;
	inobj = (Object*)ninobj;
	newobj = new(OBJECTIVE,sizeof(Object));
    setInstanceOf(newobj,inobj->instanceOf);
/*    memset(&newobj->instanceOf,0,30);
    strncpy((char*)&newobj->instanceOf,inobj->instanceOf,strlen(inobj->instanceOf));*/
	newobj->id = getNextObjectId();
    init_Object(newobj,NULL,inobj->type);
	switch(inobj->type){
	case NUMBER:
		newobj->value.number = inobj->value.number;
	break;
	case FUN:
	case CLASS:
		break;
	case STRING:
	default:
		   if(inobj->value.string == NULL) break;
		   newobj->value.string = NULL;
		   newobj->value.string = strcopy(newobj->value.string,inobj->value.string);
		break;
	}
    setAllocated(newobj,1);
	return newobj;
}
char * getInstanceOf(Object* obj){
    return obj->instanceOf;
}
void setInstanceOf(Object* obj, char* instanceof){
    memset(obj->instanceOf,0,30);
    strncpy((char*)obj->instanceOf,instanceof,strlen(instanceof));
}
