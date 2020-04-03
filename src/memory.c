/*
 * memory.c
 *
 *  Created on: Mar 20, 2020
 *      Author: scotty
 */

#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
enum MEM_TYPE mem_types;

mem_funcs mem_funcs_table= {
     &delete,
     &new,
     &init,
     &copy,
	 &resize,
    &getAllocated,
    &setAllocated,
    &setCopyConstructor,
    &setDestructor,
    &setConstructor,
	&get_header,
	&get_footer,
	1,
	0

};

void* getReference(void* obj){
    mem_footer* footer;
    footer = get_footer(obj);
    footer->functions.owner_references++;
	return obj;
}
void* releaseReference(void* obj){
	mem_footer* footer;
    footer = get_footer(obj);
	footer->functions.owner_references--;
	if(footer->functions.owner_references == 0){
		footer->functions.delete(obj);
	}
	return (void*)obj;
}
void* init(void* chunk, void* arguments){
    mem_footer * footer;
    footer= get_footer(chunk);
    return footer->functions.init(chunk,arguments);
}
void* init_memory(void* chunk, enum MEM_TYPE type, size_t size){
    mem_header * header;
    mem_footer * footer;
    void * data;
    header = (mem_header*)chunk;
    data = (void*)((char*)chunk + 2 +sizeof(mem_header));
    footer = (mem_footer*)((char*)data+size);
    header->end_canary_address = (short int*)((char*)data + size);
    header->start_canary = -69;
    footer->end_canary = -69;
    header->type = type;
    header->size = size;
    header->diff = sizeof(mem_header);
    footer->functions = mem_funcs_table;
	return data;
}

void* new(enum MEM_TYPE type, size_t size){
    void * chunk;
    void * data;
    chunk = malloc(sizeof(mem_header)+sizeof(mem_footer)+size+(char)2);
    data = init_memory(chunk,type,size);
    setAllocated(data,1);
	return data;

}

void delete(void* data){
    mem_header* header;
    mem_footer* footer;
    
	header = get_header(data);
	footer = get_footer(data);
	if(getReferenceCount(data) <= 1){
		if(header->type == RAW){
			free((void*)header);
			header = NULL;
		}
		else{
			if(footer->functions.delete != &delete){
				footer->functions.delete((void*)data);
			}
		    if(footer->functions.allocated)
			    free((void*)header);
			 header = NULL;
		}
	}
	else{
		releaseReference(data);
	}

}
short int getReferenceCount(void* obj){
    mem_footer* footer;
    footer = get_footer(obj);
	return footer->functions.owner_references;
}

void* copy(void* data){
    mem_footer* footer;
    mem_header* header;
    char* temp;
    header = get_header(data);
    footer = get_footer(data);
    if(header->type != RAW)
	return footer->functions.copy(data);
    temp = new(RAW,header->size);
    memcpy(temp,data,header->size);
    return (void*)temp;
    
}
void* resize(enum MEM_TYPE type, void* data, size_t newsize){
    void* ndata;
    mem_header* header;
    mem_footer* footer, temp;
    header = get_header(data);
    footer = get_footer(data);
    memcpy(&temp.functions,&footer->functions,sizeof(footer->functions));
    ndata = realloc((void*)header,sizeof(mem_header)+(char)2+sizeof(mem_footer)+newsize);
    header = (mem_header*)ndata;
    header->size = newsize;
    data = (void*)((char*)header + 2 + sizeof(mem_header));
    footer = (mem_footer*)((char*)data +newsize);
    footer->end_canary = -69;
    header->end_canary_address = &footer->end_canary;
    memcpy(&footer->functions,&temp.functions,sizeof(temp.functions));
    return data;


}
mem_header* get_header(void* obj){
    mem_header* header;
    header = (mem_header*)((char*)obj - (2+sizeof(mem_header)));
    return header;
}
mem_footer* get_footer(void* obj){
    mem_header* header;
    mem_footer* footer;
    header = (mem_header*)((char*)obj - (2+sizeof(mem_header)));
    footer = (mem_footer*)header->end_canary_address;
    return footer;
}
int getAllocated(void* data){
    mem_footer* footer;
    footer = get_footer(data);
    return footer->functions.allocated;
}
void setAllocated(void* data,int all){
    mem_footer* footer;
    footer = get_footer(data);
    footer->functions.allocated = all;

}

void setCopyConstructor(void* data,void*(*func)(void*)){
    mem_footer* footer;
    footer = get_footer(data);
    footer->functions.copy = func;

}
void setDestructor(void* data,void(*func)(void*)){
    mem_footer* footer;
    footer = get_footer(data);
    footer->functions.delete = func;
}
void setConstructor(void* data,void*(*func)(void*,void*)){
    mem_footer* footer;
    footer = get_footer(data);
    footer->functions.init = func;
}
