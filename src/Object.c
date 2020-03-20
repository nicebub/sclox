/*
 * Object.c
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */

#include <string.h>
#include "Object.h"
#include "TokenType.h"
#include "LoxCallable.h"
void init_Object(Object* object, void* value, TokenType type){
	if(!object) return;
	object->owner_references=1;
	object->id = getNextObjectId();
	switch(type){
	case NUMBER:
		object->value.number = *(double*)value;
		break;
	case FUN:
	case CLASS:
		object->value.callable = memcpy(malloc(sizeof(struct _LoxCallable)),value,sizeof(struct _LoxCallable));
		break;
	case STRING:
	default:
		object->value.string = strdup(value);
		break;
	}
	object->type = type;
	object->isBool = type==FALSE||type==TRUE||type==NIL?1:0;
    strcpy((char*)&object->instanceOf,"Object");
}
Object* getObjectReference(Object* obj){
	obj->owner_references += 1;
	return obj;

}
Object* releaseObjectReference(Object* obj){
	obj->owner_references -= 1;
	return NULL;
}

void delete_Object(Object** object){
	if(object && *object){
		if((*object)->owner_references <= 1){
				switch((*object)->type){
				case NUMBER:
					break;
				case FUN:
				case CLASS:
					break;
				case STRING:
				default:
					   if((*object)->value.string){
						free((*object)->value.string);
						(*object)->value.string = NULL;
					}
					break;
				}
			free(*object);
			*object = NULL;
			object = NULL;
		}
		else{
			(*object)->owner_references -= 1;
		}
	}
}
short int getNextObjectId(void){
	ocounter += 1;
	return ocounter;
}

Object* copyObject(Object* inobj){
	Object* newobj;
	newobj = malloc(sizeof(Object));
	newobj->owner_references = 1;
    memset(&newobj->instanceOf,0,30);
    strcpy((char*)&newobj->instanceOf,inobj->instanceOf);
	newobj->id = inobj->id;
	newobj->isBool = inobj->isBool;
	newobj->type = inobj->type;
	switch(inobj->type){
	case NUMBER:
		newobj->value.number = inobj->value.number;
	break;
	case FUN:
	case CLASS:
		newobj->value.callable = memcpy(malloc(sizeof(LoxCallable)),inobj->value.callable,sizeof(LoxCallable));
		break;
	case STRING:
	default:
		newobj->value.string = strdup(inobj->value.string);
		break;
	}
	return newobj;
}
