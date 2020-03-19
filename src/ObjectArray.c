
#include <stdio.h>
#include "Token.h"
#include "ObjectArray.h"
#define INIT_SIZE 5

void init_ObjectArray(ObjectArray* array){
        array->Objects = NULL;
        array->size = 0;
        array->used = 0;
        array->addElementToArray = &addElementToObjectArray;
        array->deleteArray = &delete_ObjectArray;
        array->getElementInArrayAt =&getObjectinArrayAt;
    }
            
void addElementToObjectArray(ObjectArray* array,Object* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Objects){
                array->Objects = malloc(sizeof(Object*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Objects = realloc(array->Objects, sizeof(Object*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeObjectElement(&array->Objects[i],NULL);

            initializeObjectElement(&array->Objects[array->used],element);
            array->used++;
    }
void delete_ObjectArray(ObjectArray* array){
    if(array){
        int i;
        for(i =0; i <array->used;i++){
            delete_Object(&array->Objects[i]);
        }
    }
}
Object* getObjectinArrayAt(ObjectArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Objects[index];
    return NULL;
}
