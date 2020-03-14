#ifndef _LIST_H
#define _LIST_H

#define declareArrayType(inType) \
typedef struct _##inType##Array inType##Array; \
struct _##inType##Array { \
	inType * inType##s; \
	int size; \
	int used; \
	void (*addElementToArray)(inType##Array* array,inType* element); \
	void (*deleteArray)(inType##Array* array); \
	inType* (*getElementInArrayAt)(inType##Array* array,size_t index); \
}; \
void init_##inType##Array(inType##Array* array);\
void addElementTo##inType##Array(inType##Array* array,inType* element);\
void delete##inType##Array(inType##Array* array);\
inType* get##inType##inArrayAt(inType##Array* array,size_t index); \
void initialize##inType##Element(inType* arg)


#define INIT_SIZE 5
#define defineArrayFuncs(inType) \
void init_##inType##Array(inType##Array* array){ \
		array->inType##s = NULL; \
		array->size = 0; \
		array->used = 0; \
		array->addElementToArray = &addElementTo##inType##Array; \
		array->deleteArray = &delete##inType##Array; \
		array->getElementInArrayAt =&get##inType##inArrayAt; \
	} 		\
			\
void addElementTo##inType##Array(inType##Array* array,inType* element){ \
			int i; \
			int init_start=0, init_finish=0; \
			if(!array->inType##s){ \
				array->inType##s = malloc(sizeof(inType) * INIT_SIZE); \
				init_finish = INIT_SIZE; \
				array->size = INIT_SIZE; \
			} \
			else if(array->used == array->size){ \
				array->inType##s = realloc(array->inType##s, sizeof(inType)*(array->size+INIT_SIZE));\
				init_start = array->used; \
				init_finish = array->size = array->size+INIT_SIZE; \
			} \
			for(i=init_start;i<init_finish;i++) \
				initialize##inType##Element(&array->inType##s[i]);\
\
			init_##inType(&array->inType##s[array->used],\
					element->type,element->lexeme,element->literal,element->line); \
			array->used++; \
	}\
void delete##inType##Array(inType##Array* array){\
	if(array){\
		int i; \
		for(i =0; i <array->used;i++){\
			delete_##inType(&array->inType##s[i]); \
		} \
	} \
}\
inType* get##inType##inArrayAt(inType##Array* array,size_t index){\
	if(index >= 0 && index < array->size)	\
		return &array->inType##s[index]; \
	return NULL; \
}\

#endif
