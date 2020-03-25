/*
 * memory.h
 *
 *  Created on: Mar 20, 2020
 *      Author: scotty
 */

#ifndef MEMORY_H_
#define MEMORY_H_
#include <stddef.h>

enum MEM_TYPE { RAW, OBJECTIVE };

typedef struct _mem_funcs mem_funcs;
typedef struct _mem_header mem_header;
typedef struct _mem_footer mem_footer;

struct _mem_funcs {
	void (*delete)(void * memory);
	void* (*new)(enum MEM_TYPE type, size_t size);
	void* (*init)(void* chunk, void* arguments);
	void* (*copy)(void* data);
    void* (*resize)(enum MEM_TYPE type, void* data, size_t newsize);
	int (*getAllocated)(void*);
	void (*setAllocated)(void*,int);
	void (*setCopyConstructor)(void*,void*(*)(void*));
	void (*setDestructor)(void*,void(*)(void*));
	void (*setConstructor)(void*,void*(*)(void*,void*));
	mem_header* (*get_header)(void* obj);
	mem_footer* (*get_footer)(void* obj);
	short int owner_references;
	short int allocated;
} __attribute__((packed));
struct _mem_header {
	ptrdiff_t diff;
	size_t size;
	enum MEM_TYPE type;
	short int *end_canary_address;
	short int start_canary;
}								__attribute__((packed));
struct _mem_footer {
    short int end_canary;
    mem_funcs functions;

}								__attribute__((packed));

typedef struct _mem_block mem_block;
struct _mem_block {
	void * memory;
};
typedef struct _memory memory;
struct _memory {
	mem_header header;
	mem_block memory;
    mem_footer footer;
};
void* new(enum MEM_TYPE type, size_t size);
void delete(void* data);
void* copy(void* data);
void* resize(enum MEM_TYPE type, void* data, size_t newsize);
void* init(void* chunk, void* arguments);
void* init_memory(void* chunk, enum MEM_TYPE type, size_t size);
mem_header* get_header(void* obj);
mem_footer* get_footer(void* obj);
short int getReferenceCount(void* obj);
void* getReference(void* obj);
void* releaseReference(void* obj);
void initReferenceCount(void* obj);
int getAllocated(void*);
void setAllocated(void*,int);
void setCopyConstructor(void* data,void*(*func)(void*));
void setDestructor(void* data,void(*func)(void*));
void setConstructor(void* data,void*(*func)(void*,void*));

#endif /* MEMORY_H_ */
