
#ifndef _OBJECTA_H
#define _OBJECTA_H
#include "Token.h"
#include "Object.h"

typedef struct _ObjectArray ObjectArray;
struct _ObjectArray {
    Object ** Objects;
    int size;
    int used;
    void (*addElementToArray)(ObjectArray* array,Object* element);
    void (*deleteArray)(ObjectArray* array);
    Object* (*getElementInArrayAt)(ObjectArray* array,size_t index);
};
void init_ObjectArray(ObjectArray* array);
void addElementToObjectArray(ObjectArray* array,Object* element);
void delete_ObjectArray(ObjectArray* array);
Object* getObjectinArrayAt(ObjectArray* array,size_t index);
void initializeObjectElement(Object** arg, void* values);
#endif
