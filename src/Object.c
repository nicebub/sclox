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
/*    char c;*/
/*	char c;*/
	if(!object) return;
	object->id = getNextObjectId();
/*    if(object->id == 17){
		  printf("waiting on 17\n");
		  scanf("%c",&c);
    }*/
	if(value){

		switch(type){
		    case KNULL:
			   break;
			case NUMBER:
				object->value.number = *(double*)value;
				break;
			case FUN:
			case CLASS:
/*				scanf("creatiing an object of other\n %c",&c);*/
/*		object->value.callable = memcpy(malloc(sizeof(LoxFunction)),value,sizeof(LoxFunction));*/
/*				break;*/
			case STRING:
			default:
				object->value.string = new(RAW,sizeof(char)*(strlen(value)+1));
				memset(object->value.string,0,(strlen(value)+1));
				strncpy(object->value.string,value,strlen(value));
/*		object->value.string = strdup(value);*/
				break;
		}
	}
	else object->value.string = NULL;
	object->type = type;
	object->isBool = type==FALSE||type==TRUE||type==NIL?1:0;
    memset((char*)&object->instanceOf,0,30);
    strncpy((char*)&object->instanceOf,"Object",strlen("Object"));
    object->vtable = ovtable;
    setCopyConstructor(object,&copy_Object);
    setDestructor(object,&delete_Object);
}

void delete_Object(void* inObject){
/*	char c;*/
	Object* object;
	object = (Object*)inObject;
	if(object){
		if(getReferenceCount(object) <= 1){
				switch((object)->type){
				case NUMBER:
					break;
				case FUN:
				case CLASS:
/*					scanf("found object type unknnown deleting %c\n",&c);*/
					break;
				case STRING:
				default:
					   if((object)->value.string){
/*						delete((*object)->value.string)*/;
						(object)->value.string = NULL;
					}
					break;
				}
/*			free(object);*/
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
/*	char c;*/
	Object* newobj,*inobj;
	inobj = (Object*)ninobj;
	newobj = new(OBJECTIVE,sizeof(Object));
    memset(&newobj->instanceOf,0,30);
    strncpy((char*)&newobj->instanceOf,inobj->instanceOf,strlen(inobj->instanceOf));
	newobj->id = getNextObjectId();
/*    if(newobj->id == 17){
	   printf("waiting on 17\n");
	   scanf("%c",&c);
    }*/
    init_Object(newobj,NULL,inobj->type);
	switch(inobj->type){
	case NUMBER:
		newobj->value.number = inobj->value.number;
	break;
	case FUN:
	case CLASS:
/*		   fprintf(stderr,"in copying an object also allocating memory here\n ");
		scanf("%c",&c);*/
/*		newobj->value.callable = memcpy(new(OBJECTIVE,sizeof(LoxCallable)),inobj->value.callable,sizeof(LoxCallable));*/
		break;
	case STRING:
	default:
		   if(inobj->value.string == NULL) break;
		newobj->value.string = new(RAW,sizeof(char)*(strlen(inobj->value.string)+1));
		memset(newobj->value.string,0,(strlen(inobj->value.string)+1));
		strncpy(newobj->value.string,inobj->value.string,strlen(inobj->value.string));
/*		newobj->value.string = strdup(inobj->value.string);*/
		break;
	}
    setAllocated(newobj,1);
	return newobj;
}
