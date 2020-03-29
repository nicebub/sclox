#include "hash.h"
#include "memory.h"
static struct _Hashnode_vtable hashnodeVtable = {
	     &copy_hashnode,
		&delete_hashnodel,
		&delete_hashnode,
	     &toStringNode,
		 &toStringNodeKey,
		 &toStringNodeValue,
		  NULL
};
static struct _hash_vtable hashVtable = {
		&add_to_hash,
		&copy_hash,
		&get_value_for_key,
		&get_node_for_key,
		&compute_hash_value,
		&combine_hashes,
		&print_hash,
		&delete_hash,
		&delete_hashnode,
		&remove_from_hash,
		&toString,
		NULL,
		NULL,
		NULL
};


/*struct _hr search_cache(struct _hash * s, void * key, int kt);*/


static int uniques=0;

struct _HASH *init_hash(struct _HASH* m,int size,char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value),
		short int(*compareKeysArg)(struct _HASH* h,void* key1, void* key2)){
	int i;
    m->vtable = hashVtable;
    if(toStringValueArg)
    	m->vtable.toStringValue = toStringValueArg;
    if(toStringKeyArg)
    	m->vtable.toStringKey= toStringKeyArg;
    if(compareKeysArg){
    	m->vtable.compare_keys = compareKeysArg;
    }
    m->alpha = 2;
    m->size = size;
    m->used = 0;
    m->Buckets = (struct _Hashnode**)new(OBJECTIVE,sizeof(struct _Hashnode*)*size);
    for(i=0;i<size;i++)
    	m->Buckets[i] = NULL;
    return (struct _HASH*)m;

}

struct _Hashnode* init_hashnode(struct _Hashnode* h,void * key, void* value,
		char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value)){

	h->key = key;
    h->value = value;
    h->next = NULL;
    h->vtable = hashnodeVtable;
    if(toStringValueArg)
    	h->vtable.toStringValue = toStringValueArg;
    if(toStringKeyArg)
    	h->vtable.toStringKey= toStringKeyArg;
    uniques++;
    return h;

}

struct _Hashnode *create_hashnode(void * key, void* value,
		char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value)){
    struct _Hashnode *h;
    h = new(OBJECTIVE,sizeof(*h));
    init_hashnode(h,key,value,toStringValueArg,toStringKeyArg);
    return h;
}
void add_to_hash(struct _HASH *h, void * key, void* value){
    if(h)
	   h->vtable.add_to_hash(h,key,value);
}
void* copy_hash(void* inHash){
	struct _HASH * h;
	h = (struct _HASH*) inHash;
	if(h)
	    return h->vtable.copy(h);
    return NULL;
}
struct _Hashnode * get_node_for_key(struct _HASH* h, void* key){
	if(h)
		return h->vtable.get_node_for_key(h,key);
	return NULL;
}

void* copy_hashnode(void* in_node){
    struct _Hashnode *temp;
    temp = in_node;
    if(in_node)
	   return temp->vtable.copy(in_node);
    return NULL;
}

void * get_value_for_key(struct _HASH* h, void* key){
	if(h)
	    return h->vtable.get_value_for_key(h,key);
    return NULL;
}
struct _HASH* combine_hashes(struct _HASH* h1, struct _HASH* h2){
	if(h1)
	    return h1->vtable.combine_hashes(h1,h2);
    return NULL;
}
void print_hash(struct _HASH * h){
	if(h)
	    h->vtable.print_hash(h);
}
void delete_hash(struct _HASH* h){
	if(h)
	    h->vtable.delete(h);
}
struct _Hashnode *remove_from_hash(struct _HASH* h,void* key){
	if(h)
	    return h->vtable.remove_from_hash(h,key);
    return NULL;
}
void delete_hashnode(struct _HASH* h,struct _Hashnode* n){
	if(h)
	    h->vtable.delete_node(h,n);
}
char * toString(struct _HASH* h){
	if(h)
	    return h->vtable.toString(h);
    return NULL;
}
void delete_hashnodel(struct _Hashnode* hn){
    if(hn)
		  hn->vtable.delete_hashnodel(hn);
}
char* toStringNode(struct _Hashnode* node){
    if(node)
	   return node->vtable.toString(node);
    return NULL;
}
char* toStringNodeValue(void* iNode){
	struct _Hashnode* node;
	node = (struct _Hashnode*) iNode;
    if(node)
	   return node->vtable.toStringValue(node->value);
    return NULL;
}
char* toStringNodeKey(void* iNode){
	struct _Hashnode* node;
	node = (struct _Hashnode*) iNode;
    if(node)
	   return node->vtable.toStringKey(node->key);
    return NULL;
}
short int compute_hash_value(struct _HASH * s, void * key){
	return s->vtable.compute_hash_value(s,key);
}

