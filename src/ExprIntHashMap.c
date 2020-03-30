/*
 * ExprIntHashMap.c
 *
 *  Created on: Mar 29, 2020
 *      Author: scotty
 */
#include <stdio.h>
#include "ExprIntHashMap.h"
#include "memory.h"
#include "hash.h"
#include "HashMap.h"
#include "Object.h"
#include "Expr.h"

ExprIntHashMap* init_ExprInthm(ExprIntHashMap* hm, int size){
	init_HashMap((struct _HASH*)&hm->super,size,&toStringExprInthmNodeValue,&toStringExprInthmNodeKey,&compareKeysExprInthm);
	hm->super.super.vtable.compare_keys = &compareKeysExprInthm;
	hm->super.super.vtable.toStringKey= &toStringExprInthmNodeKey;
	hm->super.super.vtable.toStringValue = &toStringExprInthmNodeValue;
	hm->super.super.vtable.compute_hash_value = &compute_hash_valueExprInthm;
    hm->super.super.vtable.copy = &copy_ExprIntHashMap;
    return  hm;
}

char *toStringExprInthmNodeValue(void* value){
if(value){
    char* num,*num2 ;
    Object * val = (Object*)value;
    if(val->type == NUMBER){
	   num = NULL;
	   asprintf(&num,"%f",val->value.number);
	   num2 = (char*)new(RAW,sizeof(char)*(strlen(num)+1));
	   memset(num2,0,strlen(num)+1);
	   strncpy(num2,num,strlen(num));
	   free(num);
	   num = NULL;
	   return num2;
    }
    else{
	   num = NULL;
	   num =new(RAW,sizeof(char)*(strlen(val->value.string)+1));
	   memset(num,0,strlen(val->value.string)+1);
	   strncpy(num,val->value.string,strlen(val->value.string));
	   return num;
    }
}
return NULL;
}
char *toStringExprInthmNodeKey(void* node){
	return "no";

}
short int compareKeysExprInthm(struct _HASH* h, void* key1, void* key2){
    Expr* one, *two;
    one = (Expr*)key1;
    two = (Expr*)key2;
	return one->id - two->id;
}
short int compute_hash_valueExprInthm(struct _HASH * hashMap,void *key){
    char *num_str;
    int h;
    short int result;
/*    int num;*/
    size_t leng;
    h=0;
    num_str=NULL;
    if(hashMap && key){
		int r;
	   Expr* expr_key;
	   expr_key = (Expr*)key;
	   num_str = NULL;
	   asprintf(&num_str,"%hd",expr_key->id);
    	leng = strlen(num_str);
    	for(r=0;r<leng;r++)
    		h = (hashMap->alpha * h) + num_str[r];
    	h = h % (int)hashMap->size;
    	result = h;
	   free(num_str);
	   num_str = NULL;
    	return result;
    }
    result = -1;
    return result;
}

ExprIntHashMap *create_ExprIntHashMap(int size){

    ExprIntHashMap *m;
/*    int i;*/
    m = new(OBJECTIVE,sizeof(ExprIntHashMap));
    init_ExprInthm(m,size);
/*
    m->super.vtable.toStringValue = toStringValueArg;
    m->super.vtable.toStringKey = toStringKeyArg;
    m->super.vtable.compare_keys = compareKeysArg;
*/
    return m;
}
void* copy_ExprIntHashMap(void* inMap){
    ExprIntHashMap* map, *nptr_map;
    int i;
    map = (ExprIntHashMap*)inMap;
    nptr_map = create_ExprIntHashMap(map->super.super.size);
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
