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
		&copy_Object,
		NULL,
	   NULL,
	   NULL
};

Object_vtable ovtable = {
		&copy_Object,
		&init_Object,
		delete_Object,
		&getNextObjectId,

};
void* new_Object(void* value, TokenType type){
	Object* obj;
    mem_footer* footer;
	obj = new(OBJECTIVE,sizeof(Object));
	init_Object(obj,value,type);
    footer = get_footer(obj);
	footer->functions.allocated = 1;
	return obj;
}
void init_Object(Object* object, void* value, TokenType type){
/*    char c;*/
    mem_footer* footer;
/*	char c;*/
	if(!object) return;
	object->id = getNextObjectId();
/*    if(object->id == 17){
		  printf("waiting on 17\n");
		  scanf("%c",&c);
    }*/
	if(value){

		switch(type){
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
    footer = get_footer(object);
    footer->functions.copy = &copy_Object;
    footer->functions.delete = &delete_Object;
    footer->functions.owner_references = 1;
}

void delete_Object(void* inObject){
/*	char c;*/
	Object** object;
	object = (Object**)inObject;
	if(object && *object){
		if(getReferenceCount(*object) <= 1){
				switch((*object)->type){
				case NUMBER:
					break;
				case FUN:
				case CLASS:
/*					scanf("found object type unknnown deleting %c\n",&c);*/
					break;
				case STRING:
				default:
					   if((*object)->value.string){
/*						delete((*object)->value.string)*/;
						(*object)->value.string = NULL;
					}
					break;
				}
			free(*object);
			*object = NULL;
			object = NULL;
		}
		else{
			releaseReference(*object);
		}
	}
}
short int getNextObjectId(void){
	ocounter += 1;
	return ocounter;
}

void* copy_Object(void* ninobj){
/*	char c;*/
    mem_footer* footer,*ofooter;
	Object* newobj,*inobj;
	inobj = (Object*)ninobj;
	newobj = new(OBJECTIVE,sizeof(Object));
    footer = get_footer(newobj);
	footer->functions.owner_references = 1;
    memset(&newobj->instanceOf,0,30);
    strncpy((char*)&newobj->instanceOf,inobj->instanceOf,strlen(inobj->instanceOf));
	newobj->id = inobj->id;
/*    if(newobj->id == 17){
	   printf("waiting on 17\n");
	   scanf("%c",&c);
    }*/
	newobj->isBool = inobj->isBool;
	newobj->type = inobj->type;
    ofooter = get_footer(inobj);
	footer->functions.delete = ofooter->functions.delete;
    footer->functions.copy = ofooter->functions.copy;
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
	return newobj;
}
