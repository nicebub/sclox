#ifndef MHASH_H
#define MHASH_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HASHMAX 256


struct _HASH;
struct _Hashnode_vtable;
struct _Hashnode;

struct _Hashnode_vtable {
    void* (*copy)(void*);
	void (*delete_hashnodel)(struct _Hashnode* hn);
	void (*delete_hashnode)(struct _HASH* h,struct _Hashnode* n);
    char* (*toString)(struct _Hashnode* node);
    char * (*toStringKey)(void* nodevalue);
    char * (*toStringValue)(void* nodevalue);
    short int (*compare_keys)(struct _HASH* h,void* key1, void* key2);
} __attribute__((packed));

struct _Hashnode {
	void* value;
	void* key;
    struct _Hashnode *next;
	struct _Hashnode_vtable vtable;
} __attribute__((packed));

struct _hash_vtable {
		void (*add_to_hash)(struct _HASH *, void * key, void* value);
		void*(*copy)(void * h);
		void * (*get_value_for_key)(struct _HASH*, void* key);
		struct _Hashnode * (*get_node_for_key)(struct _HASH* h, void* key);
		short int (*compute_hash_value)(struct _HASH * s, void * key);
		struct _HASH* (*combine_hashes)(struct _HASH*, struct _HASH*);
		void (*print_hash)(struct _HASH * h);
		void (*delete)(struct _HASH*);
		void (*delete_node)(struct _HASH*,struct _Hashnode*);
		struct _Hashnode* (*remove_from_hash)(struct _HASH* h, void*key);
		char * (*toString)(struct _HASH* h);
	   char*(*toStringValue)(void* value);
	   char*(*toStringKey)(void* value);
	   short int (*compare_keys)(struct _HASH* h,void* key1, void* key2);
} __attribute__((packed));
struct _HASH {
	struct _Hashnode** Buckets;
	struct _hash_vtable vtable;
    int size;
    int used;
    int alpha;

} __attribute__((packed));

struct _HASH *init_hash(struct _HASH* m,int size,char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value),
		short int(*compareKeys)(struct _HASH* h,void* key1, void* key2));

struct _Hashnode* init_hashnode(struct _Hashnode*,void * key, void* value,
		char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value));

struct _Hashnode *create_hashnode(void * key, void* value,
		char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value));

struct _HASH *create_hash(int size,char*(*toStringValue)(void* value),
		char*(*toStringKey)(void* value),
		short int(*compareKeys)(struct _HASH* h,void* key1, void* key2));

void add_to_hash(struct _HASH*, void * key,void * value);
char * toString(struct _HASH* h);
char* toStringNode(struct _Hashnode* node);
char* toStringNodeValue(void* node);
char* toStringNodeKey(void* node);

void*copy_hash(void* h);
void* copy_hashnode(void* in_node);

void * get_value_for_key(struct _HASH*, void* k);
struct _Hashnode * get_node_for_key(struct _HASH* h, void* key);

short int compute_hash_value(struct _HASH * s, void * key);
short int compare_keys(struct _HASH* h,void* key1, void* key2);

struct _HASH* combine_hashes(struct _HASH*, struct _HASH*);
struct _Hashnode * get_node_for_key(struct _HASH* h, void* key);
struct _Hashnode *remove_from_hash(struct _HASH*,void* kEY);

void print_hash(struct _HASH * h);
void delete_hash(struct _HASH* h);
void delete_hashnode(struct _HASH* h,struct _Hashnode* n);
void delete_hashnodel(struct _Hashnode* hn);

#endif
