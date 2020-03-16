/*
 * Object.c
 *
 *  Created on: Mar 10, 2020
 *      Author: scotty
 */


#include "Object.h"
#include "TokenType.h"

void init_Object(Object* Object, void* value, TokenType type){
	if(!Object) return;
	Object->owner_references=0;
	Object->id = getNextObjectId();
	Object->value = value;
	Object->type = type;
	Object->isBool = type==FALSE||type==TRUE||type==NIL?1:0;

}
void delete_Object(Object** object){
	if(object && *object){
		if((*object)->owner_references <= 1){
			if((*object)->value){
				free((*object)->value);
				(*object)->value = NULL;
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
