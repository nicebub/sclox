/*
 * str.c
 *
 *  Created on: Apr 3, 2020
 *      Author: scotty
 */
#include <string.h>
#include "str.h"
#include "memory.h"

char* new_str(size_t size){
	char* str;
	   str = (char*)new(RAW,sizeof(char)* (size+1));
	   memset(str,0,size+1);
	   return str;
}

char* strcopy(char* dest,const char* src){
	if(!dest){
		dest = new_str(strlen(src));
	    
	}
	strncpy(dest,src,strlen(src));
	return dest;
}
