/*
 * HashMap.h
 *
 *  Created on: Mar 15, 2020
 *      Author: scotty
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include "hash.h"

typedef struct _HashMap HashMap;
typedef struct _HashMapNode HashMapNode;
static struct _hash_vtable HashMap_vtable;
static struct _Hashnode_vtable HashMapNode_vtable;
struct _HashMap;
struct _HashMapNode;
struct _HashMap {
	struct _HASH super;
} __attribute__((packed));
struct _HashMapNode{
	struct _Hashnode super;

} __attribute__((packed));

struct _HASH *init_HashMap(struct _HASH* m,int size,char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value),
		short int(*compareKeys)(struct _HASH* h,void* key1, void* key2));

struct _Hashnode* init_HashMapNode(struct _Hashnode*,void * key, void* value,
		char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value));


HashMap *create_HashMap(int size,char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value),
		short int(*compareKeysArg)(struct _HASH* h,void* key1, void* key2));

HashMapNode* create_HashMapNode(void*key,void*value,
		char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void*));


void add_to_HashMap(struct _HASH*, void * key,void * value);

void *copy_HashMap(void* h);
void* copy_HashMapNode(void* node);
void * get_value_for_keyHashMap(struct _HASH*, void* k);
struct _Hashnode * get_node_for_keyHashMap(struct _HASH* h, void* key);

struct _HASH* combine_hashesHashMap(struct _HASH*, struct _HASH*);
short int compute_hash_valueHashMap(struct _HASH * hashMap,void *key);
struct _Hashnode* remove_from_HashMap(struct _HASH* h, void*key);
char * toStringHashMap(struct _HASH* h);
char* toStringHashMapNode(struct _Hashnode* node);
char *toStringHashMapNodeValue(void* node);
char *toStringHashMapNodeKey(void* node);
short int compareKeysHashMap(struct _HASH* h,void* key1, void* key2);

void delete_HashMap(struct _HASH* h);
void print_HashMap(struct _HASH * h);
void delete_HashMapNode(struct _HASH* h,struct _Hashnode* n);

void delete_HashMapNodeList(struct _Hashnode* hn);


short int compute_hash_valueHashMap(struct _HASH * h,void *key);

#endif /* HASHMAP_H_ */
