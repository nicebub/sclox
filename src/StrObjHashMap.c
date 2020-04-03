/*
 * StrObjHashMap.c
 *
 *  Created on: Mar 28, 2020
 *      Author: scotty
 */
#include "memory.h"
#include "hash.h"
#include "HashMap.h"
#include "StrObjHashMap.h"
#include "Object.h"
#include "str.h"
StrObjHashMap* init_StrObjhm(StrObjHashMap* hm, int size){
	init_HashMap((struct _HASH*)&hm->super,size,&toStringStrObjhmNodeValue,&toStringStrObjhmNodeKey,&compareKeysStrObjhm);
	hm->super.super.vtable.compare_keys = &compareKeysStrObjhm;
	hm->super.super.vtable.toStringKey= &toStringStrObjhmNodeKey;
	hm->super.super.vtable.toStringValue = &toStringStrObjhmNodeValue;
	hm->super.super.vtable.compute_hash_value = &compute_hash_valueStrObjhm;
    hm->super.super.vtable.copy = &copy_StrObjHashMap;
    return  hm;
}

char *toStringStrObjhmNodeValue(void* value){
if(value){
    char* num,*num2 ;
    Object * val = (Object*)value;
    if(val->type == NUMBER){
	   num = NULL;
	   asprintf(&num,"%f",val->value.number);
	   num2 = NULL;
	   num2 = strcopy(num2,num);
/*	   num2 = (char*)new(RAW,sizeof(char)*(strlen(num)+1));
	   memset(num2,0,strlen(num)+1);
	   strncpy(num2,num,strlen(num));*/
	   free(num);
	   num = NULL;
	   return num2;
    }
    else{
	   num = NULL;
	   num = strcopy(num,val->value.string);
/*	   num =new(RAW,sizeof(char)*(strlen(val->value.string)+1));
	   memset(num,0,strlen(val->value.string)+1);
	   strncpy(num,val->value.string,strlen(val->value.string));*/
	   return num;
    }
}
return NULL;
}
char *toStringStrObjhmNodeKey(void* node){
	return "no";

}
short int compareKeysStrObjhm(struct _HASH* h, void* key1, void* key2){
	return strcmp(key1,key2);
}
short int compute_hash_valueStrObjhm(struct _HASH * hashMap,void *key){
    char *string;
    int h;
    short int result;
    size_t leng;
    h=0;
    if(hashMap && key){
		int r;
    	string = (char*)key;
    	leng = strlen(key);
    	for(r=0;r<leng;r++)
    		h = (hashMap->alpha * h) + string[r];
    	h = h % (int)hashMap->size;
    	result = h;
    	return result;
    }
    result = -1;
    return result;
}

StrObjHashMap *create_StrObjHashMap(int size){

    StrObjHashMap *m;
    m = new(OBJECTIVE,sizeof(StrObjHashMap));
    init_StrObjhm(m,size);
    return m;
}
void* copy_StrObjHashMap(void* inMap){
    StrObjHashMap* map, *nptr_map;
    int i;
    map = (StrObjHashMap*)inMap;
    nptr_map = create_StrObjHashMap(map->super.super.size);
    for(i=0;i<map->super.super.size;i++){
	   struct _Hashnode* temp;
	   temp = map->super.super.Buckets[i];
	   while(temp != NULL){
		  add_to_HashMap((struct _HASH*)nptr_map,copy(temp->key),copy(temp->value));
		  temp = temp->next;
	   }
    }
    return nptr_map;
}
