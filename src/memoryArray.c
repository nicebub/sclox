
#include <stdio.h>
#include "Token.h"
#include "memoryArray.h"
#define INIT_SIZE 5

void init_memoryArray(memoryArray* array){
        array->memorys = NULL;
        array->size = 0;
        array->used = 0;
        array->memory.owner_references =1;
        array->addElementToArray = &addElementTomemoryArray;
        array->delete = &delete_memoryArray;
        array->getElementInArrayAt =&getmemoryinArrayAt;
        array->copy = &copymemoryArray;
    }
            
void addElementTomemoryArray(memoryArray* array,memory* element){
            int i;
            int init_start=0, init_finish=0;
            if(!array->memorys){
                array->memorys = malloc(sizeof(memory) * INIT_SIZE);
                init_finish = INIT_SIZE;
                array->size = INIT_SIZE;
            }
            else if(array->used == array->size){
                array->memorys = realloc(array->memorys, sizeof(memory)*(array->size+INIT_SIZE));
                init_start = array->used;
                init_finish = array->size = array->size+INIT_SIZE;
            }
            for(i=init_start;i<init_finish;i++)
                initializememoryElement(&array->memorys[i],NULL);

            initializememoryElement(&array->memorys[array->used],element);
            array->used++;
    }
void delete_memoryArray(memoryArray* array){
    if(array){
        int i;
        if(array->memory.owner_references<=1){
            for(i =0; i <array->used;i++){
/*                delete(&array->memorys[i]);*/
            }
        }
        else{
            releaseReference(array);
        }
    }
}
memory* getmemoryinArrayAt(memoryArray* array,size_t index){
    if(index >= 0 && index < array->size)
        return &array->memorys[index];
    return NULL;
}
void*  copymemoryArray(void * inArr){
    memoryArray* newarr,*arr;
    int i;
    arr = (memoryArray*)inArr;
    newarr = malloc(sizeof(memoryArray));
    init_memoryArray(newarr);
    for(i=0;i<arr->used;i++){
        newarr->addElementToArray(newarr, copy(getmemoryinArrayAt(arr,i)));
    }
    newarr->memory.allocated =1 ;
    return newarr;
}


