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
static struct _hash_vtable hashmap_vtable;
static struct _Hashnode_vtable HashNodeMap_vtable;
struct _HashMap;
struct _HashMapNode;
struct _HASH *create_hashmap(int size,char*(*func)(void* value));
void add_to_hashmap(struct _HASH*, void * key,void * value);

void *copy_hashmap(void* h);
void* copy_hashmapnode(void* node);
void * get_value_for_keymap(struct _HASH*, void* k);
struct _Hashnode * get_node_for_keymap(struct _HASH* h, void* key);

struct _HASH* combine_hashesmap(struct _HASH*, struct _HASH*);
short int compute_hash_valuemap(struct _HASH * hashMap,void *key);
struct _Hashnode* remove_from_hashmap(struct _HASH* h, void*key);
char * toStringMap(struct _HASH* h);
char* toStringMapNode(struct _Hashnode* node);
char *toStringMapNodeValue(struct _Hashnode* node);

void delete_hashmap(struct _HASH* h);
void print_hashmap(struct _HASH * h);
void delete_hashnodemap(struct _HASH* h,struct _Hashnode* n);

void delete_hashnodelmap(struct _Hashnode* hn);
void set_hnode_valuemap(struct _Hashnode*, void * v);
void set_hnode_keymap(struct _Hashnode*, void * k);
void * get_hnode_valuemap(struct _Hashnode*);
void * get_hnode_keymap(struct _Hashnode*);
int get_hnode_value_typemap(struct _Hashnode* hn);
int get_hnode_key_typemap(struct _Hashnode* hn);

HashMapNode* create_hashnodemap(void*key,void*value,char*(*func)(void* value) );

short int compute_hash_valuemap(struct _HASH * h,void *key);


struct _HashMap {
	struct _HASH super;
} __attribute__((packed));
struct _HashMapNode{
	struct _Hashnode super;

} __attribute__((packed));

#endif /* HASHMAP_H_ */
