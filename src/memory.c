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
/*
struct _mem_funcs func_vtable = {
};*/
enum MEM_TYPE mem_types;
void* getReference(void* obj){
/*    char c;*/
    mem_footer* footer;
/*    printf("sizeof mem_header %lu\n",sizeof(mem_header));*/
/*    scanf("%c",&c);*/
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

void* init_memory(void* chunk, enum MEM_TYPE type, size_t size){
/*    char c;*/
    mem_header * header;
    mem_footer * footer;
    void * data;
    header = (mem_header*)chunk;
    data = (void*)((char*)chunk + 2 +sizeof(mem_header));
    footer = (mem_footer*)((char*)data+size);
    header->end_canary_address = (short int*)((char*)data + size);
    header->size = size;
    header->start_canary = -69;
    header->type = type;
    footer->end_canary = -69;
    header->size = size;
    header->diff = sizeof(mem_header);
    footer->functions.allocated = 0;
    footer->functions.delete = &delete;
    footer->functions.new = &new;
    footer->functions.copy = &copy;
    footer->functions.owner_references = 1;
    footer->functions.get_header = &get_header;
    footer->functions.get_footer = &get_footer;
/*	printf("in init sizeof mem_header %lu\n",sizeof(mem_header));*/
/*	scanf("%c",&c);*/
	return data;
}

void* new(enum MEM_TYPE type, size_t size){
    void * chunk;
    void * data;
/*    char c;*/
    mem_footer* footer;
    chunk = malloc(sizeof(mem_header)+sizeof(mem_footer)+size+(char)2);
    data = init_memory(chunk,type,size);
    footer = (mem_footer*)((char*)data+size);
	footer->functions.allocated = 1;
/*	printf("in new sizeof mem_header %lu\n",sizeof(mem_header));*/
/*	scanf("%c",&c);*/
	return data;

}

void delete(void* data){
    mem_header* header;
    mem_footer* footer;
    
/*	mem_header * head;*/
	header = get_header(data);
	footer = get_footer(data);
/*	ncell = (memory*)head;*/
/*	fprintf(stderr, "found memory node: start canary %d send canary %d\n",
			header->start_canary,footer->end_canary);
	fprintf(stderr, "size %ld and difference %ld\n",header->size,
			header->diff);*/
	if(getReferenceCount(data) <= 1){
		if(header->type == RAW){
			free((void*)header);
			header = NULL;
		}
		else{
			if(footer->functions.delete != &delete){
				footer->functions.delete((void*)data);
			}
		}
	    /*
		if(footer->functions.allocated)
			free((void*)header);
		  header = NULL;
*/
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
    footer = get_footer(data);
	return footer->functions.copy(data);
}
void* resize(enum MEM_TYPE type, void* data, size_t newsize){
    void* ndata;
    mem_header* header;
    mem_footer* footer, temp;
    header = get_header(data);
    footer = get_footer(data);
    memcpy(&temp.functions,&footer->functions,sizeof(footer->functions));
/*    temp.functions = footer->functions;*/
/*    fprintf(stderr,"header before realloc: header->%d\n",header->start_canary);*/
    ndata = realloc((void*)header,sizeof(mem_header)+(char)2+sizeof(mem_footer)+newsize);
    header = (mem_header*)ndata;
    header->size = newsize;
/*    fprintf(stderr,"header after realloc: header->%d\n",header->start_canary);*/
/*    fprintf(stderr,"sizeof: %lu \n",sizeof(ndata));*/
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
