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

	Object->value = value;
	Object->type = type;
	Object->isBool = type==FALSE||type==TRUE?1:0;

}
void delete_Object(Object** object){
	if(object && *object){
		if((*object)->value){
			free((*object)->value);
			(*object)->value = NULL;
		}
		free(*object);
		*object = NULL;
	    object = NULL;
	}
}
