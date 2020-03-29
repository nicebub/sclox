/*
 * StrObjHashMap.h
 *
 *  Created on: Mar 28, 2020
 *      Author: scotty
 */

#ifndef STROBJHASHMAP_H_
#define STROBJHASHMAP_H_
#include "hash.h"
#include "HashMap.h"
typedef struct _StrObjHashMap StrObjHashMap;

struct _StrObjHashMap {
	HashMap super;
};

StrObjHashMap* init_StrObjhm(StrObjHashMap* hm, int size);

char *toStringStrObjhmNodeValue(void* node);
char *toStringStrObjhmNodeKey(void* node);
short int compareKeysStrObjhm(struct _HASH* h,void* key1, void* key2);
short int compute_hash_valueStrObjhm(struct _HASH * h,void *key);
StrObjHashMap *create_StrObjHashMap(int size);
void* copy_StrObjHashMap(void* inMap);

#endif /* STROBJHASHMAP_H_ */
