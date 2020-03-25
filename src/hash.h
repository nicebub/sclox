#ifndef MHASH_H
#define MHASH_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HASHMAX 256

#define value_from_int(h,k) get_value_for_key(h,k,0)
#define value_from_char(h,k) get_value_for_key(h,k,2)
#define value_from_string(h,k) get_value_for_key(h,k,3)

#define add_itoh(h,v,k,kt) add_to_hash(h,v,0,k,kt)
#define add_ctoh(h,v,k,kt) add_to_hash(h,v,2,k,kt)
#define add_stoh(h,v,k,kt) add_to_hash(h,v,3,k,kt)

#define add_iitoh(h,v,k) add_to_hash(h,v,0,k,0)
#define add_ictoh(h,v,k) add_to_hash(h,v,0,k,2)
#define add_istoh(h,v,k) add_to_hash(h,v,0,k,3)
#define add_citoh(h,v,k) add_to_hash(h,v,2,k,0)
#define add_cctoh(h,v,k) add_to_hash(h,v,2,k,2)
#define add_cstoh(h,v,k) add_to_hash(h,v,2,k,3)
#define add_sitoh(h,v,k) add_to_hash(h,v,3,k,0)
#define add_sctoh(h,v,k) add_to_hash(h,v,3,k,2)
#define add_sstoh(h,v,k) add_to_hash(h,v,3,k,3)

struct _HASH;
struct _Hashnode_vtable;
struct _Hashnode;

struct _Hashnode_vtable {
    void* (*copy)(void*);
	void (*delete_hashnodel)(struct _Hashnode* hn);
	void (*set_hnode_value)(struct _Hashnode*, void * v);
	void (*set_hnode_key)(struct _Hashnode*, void * k);
	void * (*get_hnode_value)(struct _Hashnode*);
	void * (*get_hnode_key)(struct _Hashnode*);
	int (*get_hnode_value_type)(struct _Hashnode* hn);
	int (*get_hnode_key_type)(struct _Hashnode* hn);
    char* (*toString)(struct _Hashnode* node);
    char * (*toStringValue)(struct _Hashnode* nodevalue);
    char*(*func)(void* value);
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
	   char*(*func)(void* value);
} __attribute__((packed));
struct _HASH {
	struct _Hashnode** Buckets;
	struct _hash_vtable vtable;
    int size;
    int used;
    int alpha;

} __attribute__((packed));
struct _Hashnode *create_hashnode(void * key, void* value,char*(*func)(void* value));

struct _HASH *create_hash(int size,char*(*func)(void* value));
void add_to_hash(struct _HASH*, void * key,void * value);
char * toString(struct _HASH* h);
char* toStringNode(struct _Hashnode* node);
char* toStringNodeValue(struct _Hashnode* node);

struct _HASH *copy_hash(struct _HASH* h);
void* copy_hashnode(void* in_node);

void * get_value_for_key(struct _HASH*, void* k);
struct _Hashnode * get_node_for_key(struct _HASH* h, void* key);

short int compute_hash_value(struct _HASH * s, void * key);

struct _HASH* combine_hashes(struct _HASH*, struct _HASH*);
struct _Hashnode *remove_from_hash(struct _HASH*,void* kEY);

void delete_hash(struct _HASH* h);
void print_hash(struct _HASH * h);
void delete_hashnode(struct _HASH* h,struct _Hashnode* n);

struct _hashnode {
    void * value;
    void * key;
    struct _hashnode *next;
    int uniq;
    int vtype;
    int ktype;
} __attribute__((packed));

typedef struct _hashnode ** hashbucket_array;

struct _ca{
    void* p;
    int pt;
    int r;
};

struct _hr{
    int r;
    int i;
};
/*typedef struct _ca ** ca;*/

struct _hash {
    hashbucket_array bucket;
    struct _ca *cache;
    int csize;
    int cused;
    int size;
    int used;
 /*   int lasth;*/
    int alpha;
} __attribute__((packed));

#endif
