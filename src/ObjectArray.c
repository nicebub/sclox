
#include <stdio.h>
#include "Token.h"
#include "ObjectArray.h"
#define INIT_SIZE 5

void init_ObjectArray(ObjectArray* array){
        array->Objects = NULL;
        array->size = 0;
        array->used = 0;
	   setAllocated(array,0);
	   setCopyConstructor(array,&copyObjectArray);
	   setDestructor(array,&delete_ObjectArray);
        array->addElementToArray = &addElementToObjectArray;
        array->delete = &delete_ObjectArray;
        array->getElementInArrayAt =&getObjectinArrayAt;
        array->copy = &copyObjectArray;
    }
            
void addElementToObjectArray(ObjectArray* array,Object* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->Objects){
                array->Objects = new(OBJECTIVE,sizeof(Object*) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->Objects = resize(OBJECTIVE,array->Objects, sizeof(Object*)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializeObjectElement(&array->Objects[i],NULL);

            initializeObjectElement(&array->Objects[array->used],element);
            array->used++;
    }
void delete_ObjectArray(void* inArray){
    ObjectArray* array;
    array = (ObjectArray*) inArray;
    if(array){
        int i;
        if(getReferenceCount(array)<=1){
            for(i =0; i <array->used;i++){
                delete(&array->Objects[i]);
            }
        }
        else{
            releaseReference(array);
        }
    }
}
Object* getObjectinArrayAt(ObjectArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return array->Objects[index];
    return NULL;
}
void*  copyObjectArray(void * inArr){
    ObjectArray* newarr,*arr;
    int i;
    arr = (ObjectArray*)inArr;
    newarr = new(OBJECTIVE,sizeof(ObjectArray));
    init_ObjectArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, copy(getObjectinArrayAt(arr,i)));
    }
    setAllocated(newarr,1);
    return newarr;
}


