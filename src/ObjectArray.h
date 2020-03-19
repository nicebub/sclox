
#ifndef _OBJECTA_H
#define _OBJECTA_H
#include "Token.h"
#include "Object.h"
#ifndef _EXPRARRAY
#define _EXPRARRAY
    typedef struct _ExprArray ExprArray;
    extern void delete_ExprArray(ExprArray* array);
#endif

typedef struct _ObjectArray ObjectArray;

struct _ObjectArray {
    Object ** Objects;
    int owner_references;
    int size;
    int used;
    void (*addElementToArray)(ObjectArray* array,Object* element);
    void (*deleteArray)(ObjectArray* array);
    Object* (*getElementInArrayAt)(ObjectArray* array,size_t index);
    ObjectArray*  (*copyArray)(ObjectArray * arr);
    ObjectArray* (*getArrayReference)(ObjectArray* arr);
    ObjectArray* (*releaseArrayReference)(ObjectArray* arr);
    
};
void init_ObjectArray(ObjectArray* array);
void addElementToObjectArray(ObjectArray* array,Object* element);
void delete_ObjectArray(ObjectArray* array);
Object* getObjectinArrayAt(ObjectArray* array,size_t index);
void initializeObjectElement(Object** arg, void* values);
ObjectArray*  copyObjectArray(ObjectArray * arr);
ObjectArray* getObjectArrayReference(ObjectArray* arr);
ObjectArray* releaseObjectArrayReference(ObjectArray* arr);
#endif
