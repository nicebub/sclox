
#ifndef _OBJECTA_H
#define _OBJECTA_H
#include "Token.h"
#include "Object.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(void* array);
#endif

typedef struct _ObjectArray ObjectArray;

struct _ObjectArray {
    Object ** Objects;
    int size;
    int used;
    void (*addElementToArray)(ObjectArray* array,Object* element);
    void (*delete)(void* array);
    Object* (*getElementInArrayAt)(ObjectArray* array,size_t index);
    void*  (*copy)(void * arr);
    
};
void init_ObjectArray(ObjectArray* array);
void addElementToObjectArray(ObjectArray* array,Object* element);
void delete_ObjectArray(void* array);
Object* getObjectinArrayAt(ObjectArray* array,size_t index);
void initializeObjectElement(Object** arg, void* values);
void*  copyObjectArray(void * arr);
#endif
