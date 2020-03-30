/*
 * ExprIntHashMap.h
 *
 *  Created on: Mar 29, 2020
 *      Author: scotty
 */

#ifndef EXPRINTHASHMAP_H_
#define EXPRINTHASHMAP_H_
#include "hash.h"
#include "HashMap.h"
typedef struct _ExprIntHashMap ExprIntHashMap;

struct _ExprIntHashMap {
	HashMap super;
};

ExprIntHashMap* init_ExprInthm(ExprIntHashMap* hm, int size);

char *toStringExprInthmNodeValue(void* node);
char *toStringExprInthmNodeKey(void* node);
short int compareKeysExprInthm(struct _HASH* h,void* key1, void* key2);
short int compute_hash_valueExprInthm(struct _HASH * h,void *key);
ExprIntHashMap *create_ExprIntHashMap(int size);
void* copy_ExprIntHashMap(void* inMap);

#endif /* EXPRINTHASHMAP_H_ */
