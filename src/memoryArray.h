
#ifndef _MEMORYA_H
#define _MEMORYA_H
#include "Token.h"

typedef struct _memoryArray memoryArray;

struct _memoryArray {
    mem_funcs memory;
    memory * memorys;
    int size;
    int used;
    void (*addElementToArray)(memoryArray* array,memory* element);
    void (*delete)(memoryArray* array);
    memory* (*getElementInArrayAt)(memoryArray* array,size_t index);
    void*  (*copy)(void * arr);
    
};
void init_memoryArray(memoryArray* array);
void addElementTomemoryArray(memoryArray* array,memory* element);
void delete_memoryArray(memoryArray* array);
memory* getmemoryinArrayAt(memoryArray* array,size_t index);
void initializememoryElement(memory* arg, void* values);
void*  copymemoryArray(void * arr);
#endif
