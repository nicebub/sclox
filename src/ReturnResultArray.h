
#ifndef _RETURNRESULTA_H
#define _RETURNRESULTA_H
#include "Token.h"

typedef struct _ReturnResultArray ReturnResultArray;
struct _ReturnResultArray {
    ReturnResult * ReturnResults;
    int size;
    int used;
    void (*addElementToArray)(ReturnResultArray* array,ReturnResult* element);
    void (*deleteArray)(ReturnResultArray* array);
    ReturnResult* (*getElementInArrayAt)(ReturnResultArray* array,size_t index);
};
void init_ReturnResultArray(ReturnResultArray* array);
void addElementToReturnResultArray(ReturnResultArray* array,ReturnResult* element);
void delete_ReturnResultArray(ReturnResultArray* array);
ReturnResult* getReturnResultinArrayAt(ReturnResultArray* array,size_t index);
void initializeReturnResultElement(ReturnResult* arg, void* values);
#endif
