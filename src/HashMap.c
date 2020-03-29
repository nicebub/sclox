/*
 * HashMap.c
 *
 *  Created on: Mar 15, 2020
 *      Author: scotty
 */

#include "HashMap.h"
#include "hash.h"
#include "memory.h"
static struct _Hashnode_vtable HashNodeMap_vtable = {
	     &copy_HashMapNode,
		&delete_HashMapNodeList,
		&delete_HashMapNode,
	     &toStringHashMapNode,
		 &toStringHashMapNodeKey,
		 &toStringHashMapNodeValue,
		  &compareKeysHashMap

};
static struct _hash_vtable hashmap_vtable = {
		&add_to_HashMap,
		&copy_HashMap,
		&get_value_for_keyHashMap,
		&get_node_for_keyHashMap,
		&compute_hash_valueHashMap,
		&combine_hashesHashMap,
		&print_HashMap,
		&delete_HashMap,
		&delete_HashMapNode,
		&remove_from_HashMap,
		&toStringHashMap,
	   &toStringHashMapNodeValue,
	   &toStringHashMapNodeKey,
	   &compareKeysHashMap

};

struct _HASH *init_HashMap(struct _HASH* m,int size,char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value),
		short int(*compareKeysArg)(struct _HASH* h,void* key1, void* key2)){
	HashMap * hm;
	int i;
	hm = (HashMap*) m;
	init_hash(&hm->super,size,toStringValueArg,toStringKeyArg,compareKeysArg);
    hm->super.vtable = hashmap_vtable;
    if(toStringValueArg)
    	m->vtable.toStringValue = toStringValueArg;
    if(toStringKeyArg)
    	m->vtable.toStringKey= toStringKeyArg;
    if(compareKeysArg){
    	m->vtable.compare_keys = compareKeysArg;
    }
    delete(hm->super.Buckets);
    hm->super.Buckets =(struct _Hashnode**)new(OBJECTIVE,sizeof(HashMapNode*)*size);
    for(i =0;i<size;i++)
	   hm->super.Buckets[i] = NULL;
    return (struct _HASH*)hm;
}

struct _Hashnode* init_HashMapNode(struct _Hashnode* n,void * key, void* value,
		char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value)){
	HashMapNode* h;
	h = (HashMapNode*)n;
	init_hashnode(&h->super,key,value,toStringValueArg,toStringKeyArg);
   	  h->super.vtable = HashNodeMap_vtable;
      if(toStringValueArg)
      	h->super.vtable.toStringValue = toStringValueArg;
      if(toStringKeyArg)
      	h->super.vtable.toStringKey= toStringKeyArg;
	return (struct _Hashnode*)h;

}

HashMap *create_HashMap(int size,char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value),
		short int(*compareKeysArg)(struct _HASH* h,void* key1, void* key2)){

    HashMap *m;
/*    int i;*/
    m = new(OBJECTIVE,sizeof(HashMap));
    init_HashMap(&m->super,size,toStringValueArg,toStringKeyArg,compareKeysArg);
/*
    m->super.vtable.toStringValue = toStringValueArg;
    m->super.vtable.toStringKey = toStringKeyArg;
    m->super.vtable.compare_keys = compareKeysArg;
*/
    return m;
}
void add_to_HashMap(struct _HASH* h, void * key,void * value){
/*    struct _hr r;*/
    short int res;
    HashMapNode *hn;
    HashMapNode *temp;
    HashMap *hm;
    if(h && key){

	   if(get_value_for_keyHashMap(h,key)!=NULL){
		   HashMapNode* ex;
		   ex = (HashMapNode*)get_node_for_keyHashMap(h,key);
		   delete(ex->super.value);
		   ex->super.value = getReference(value);
		   return;
	   }
	   hm = (HashMap*)h;
	   hn = (HashMapNode*) create_HashMapNode(getReference(key),getReference(value),
			   h->vtable.toStringValue, h->vtable.toStringKey);
	   res = compute_hash_valueHashMap(h,key);
	   if(hm->super.Buckets[res] == NULL){
		  hm->super.Buckets[res] = (struct _Hashnode*) hn;
	   }
	   
	   else{
		  temp = (HashMapNode*)((HashMap*)h)->super.Buckets[res];
		  while(temp->super.next != NULL)
			 temp = (HashMapNode*)temp->super.next;
		  temp->super.next = (struct _Hashnode*)hn;
	   }
	   h->used++;
}
}

HashMapNode* create_HashMapNode(void*key,void*value,char*(*toStringValueArg)(void* value),
		char*(*toStringKeyArg)(void* value) ){

	HashMapNode *h;
	    h = new(OBJECTIVE,sizeof(HashMapNode));
	    init_HashMapNode((struct _Hashnode*)h,key,value,toStringValueArg,toStringKeyArg);

    /*
    h->super.vtable.delete_hashnodel = &delete_hashnodelmap;
	    h->super.vtable.get_hnode_key = &get_hnode_keymap;
	    h->super.vtable.get_hnode_key_type = &get_hnode_key_typemap;
	    h->super.vtable.get_hnode_value = &get_hnode_valuemap;
	    h->super.vtable.get_hnode_value_type = &get_hnode_value_typemap;
	    h->super.vtable.set_hnode_key = &set_hnode_keymap;
	    h->super.vtable.set_hnode_value = &set_hnode_valuemap;
    */
/*	    uniques++;*/
	    return h;

}
short int compareKeysHashMap(struct _HASH* h,void* key1, void* key2){
    if(h)
	   return h->vtable.compare_keys(h,key1,key2);
    return -1;
}

short int compute_hash_valueHashMap(struct _HASH * hashMap,void *key){
	if(hashMap)
		return hashMap->vtable.compute_hash_value(hashMap,key);
	return -1;
}
short int compute_hash_valueString(struct _HASH * hashMap,void *key){
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

void * get_value_for_keyHashMap(struct _HASH* h, void* key){
	short int res;
	struct _Hashnode *temp;

	if(h && key){
		res = compute_hash_valueHashMap(h,key);
		temp = ((HashMap*)h)->super.Buckets[res];
		while(temp != NULL){
			if(compareKeysHashMap(h,temp->key,key)==0)
				return temp->value;
			temp = temp->next;
		}
	}
	return NULL;
}
struct _Hashnode * get_node_for_keyHashMap(struct _HASH* h, void* key){
	short int res;
	struct _Hashnode *temp;
	if(h && key){
		res = compute_hash_valueHashMap(h,key);
		temp = ((HashMap*)h)->super.Buckets[res];
		while(temp != NULL){
			if(compareKeysHashMap(h,temp->key,key)==0)
				return temp;
			temp = temp->next;
		}
	}
	return NULL;
}
char* toStringHashMapNodeValue(void* iNode){
	struct _Hashnode* node;
	node = (struct _Hashnode*) iNode;
    if(node)
	   return node->vtable.toStringValue(node->value);
    return NULL;
}
char* toStringHashMapNodeKey(void* iNode){
	struct _Hashnode * node;
	node = (struct _Hashnode*) iNode;
    if(node)
	   return node->vtable.toStringKey(node->value);
    return NULL;
}
/*
short int compare_keysMap(struct _Hashnode* node){
    if(node)
	   return node->vtable.toStringValue(node->value);
    return NULL;
}*/
char* toStringHashMapNode(struct _Hashnode* node){
    char* temp;
    char * num;
    char* keystr;
    size_t str_len;
    num = NULL;
    num = new(RAW,sizeof(char)*(strlen(toStringHashMapNodeValue(node))+1));
    asprintf(&num,"%s",toStringHashMapNodeValue(node));
    str_len = (4+strlen(num)+strlen((char*)node->key));
    temp = new(RAW,sizeof(char)*str_len);
    memset(temp,0,str_len);
    strncat(temp,"(",1);
    keystr = toStringHashMapNodeKey(node);
    strncat(temp,keystr,strlen(keystr));
    strncat(temp,",",1);
    strncat(temp,num,strlen(num));
    strncat(temp,")",1);
    delete(keystr);
    keystr = NULL;
    delete(num);
    num = NULL;
    return temp;
}
char* toStringHashMap(struct _HASH* h){
    int fullstr_len,i;
    int count;
    char* temp, *num;
    char extra[] = "()";
    char each[] = "(,),";
    fullstr_len=0;
    num = NULL;
    for(i=0;i<h->size;i++){
	   HashMapNode* node = (HashMapNode*)((HashMap*)h)->super.Buckets[i];
	   if(node){
		  while(node){
			 num = toStringHashMapNodeValue((struct _Hashnode*)node);
/*			 asprintf(&num,"%d",*(int*)node->super.value);*/
			 fullstr_len += strlen(node->super.key);
			 fullstr_len += strlen(num);
/*			 delete(num);*/
			 num= NULL;
			 node= (HashMapNode*)node->super.next;
		  }
	   }
    }
    fullstr_len= (int)fullstr_len+1+(int)strlen(extra)+((int)strlen(each)*h->used+(h->used-1));
    temp = new(RAW,sizeof(char)*fullstr_len);
    memset(temp,0,fullstr_len);
    temp[0]='(';
    count = 0;
    for(i=0;i<h->size;i++){
	   HashMapNode* node = (HashMapNode*)((HashMap*)h)->super.Buckets[i];
	   if(node){
		  while(node){
			 char * nstr = toStringHashMapNode((struct _Hashnode*)node);
			 count++;
			 strncat(temp,nstr,fullstr_len-strlen(temp));
				if(node->super.next !=NULL)
				    strncat(temp,",",1);
			 node= (HashMapNode*)node->super.next;
/*			 delete(nstr);*/
			 nstr = NULL;
		  }
		  if(count < h->used)
				strncat(temp,",",1);
	   }
    }
    temp[strlen(temp)]=')';
    temp[strlen(temp)]='\0';
	return temp;
}
void delete_HashMap(struct _HASH* h){
    if(h){
	   if(getReferenceCount(h) <= 1){
		  int t;
		  for(t=0;t<h->size;t++){
				if(h->Buckets[t]){
				    HashMapNode* temp;
/*				    HashMapNode* next;*/
				    temp = (HashMapNode*)h->Buckets[t];
/*				    while(temp){
					   next = (HashMapNode*)temp->super.next;*/
					   delete_HashMapNodeList((struct _Hashnode*)temp);
/*					   temp = NULL;
					   temp = next;
				    }*/
				}
				h->Buckets[t] = NULL;
		  }
		  if(getAllocated(h)){
			 delete(h);
		  }
	   }
	   else{
		  releaseReference(h);
	   }
/*	   for(a=1;a<=h->csize;a++){
		  free(h->cache[a-1].p);
		  h->cache[a-1].p = NULL;
	   }*/
/*	   free(h->cache);
	   h->cache = NULL;*/
/*	   delete(h);*/
	   h = NULL;
    }

}
void print_HashMap(struct _HASH * h){
    fprintf(stderr,"%s\n",h->vtable.toString(h));
    
}
void delete_HashMapNode(struct _HASH* h,struct _Hashnode* hn){
    if(hn){
	   if(getReferenceCount(hn) <= 1 ){
		  HashMapNode* hnm = (HashMapNode*) hn;
		  delete(hnm->super.key);
		  hnm->super.key = NULL;
		  delete(hnm->super.value);
		  hnm->super.value = NULL;
		  hnm->super.next = NULL;
		  if(getAllocated(hn)){
				delete(hnm);
		  }
		  hnm = NULL;
		  hn = NULL;
	   }
	   else{
		  releaseReference(hn);
	   }
    }
}

void delete_HashMapNodeList(struct _Hashnode* hn){
	   if(hn){
		  if(getReferenceCount(hn) <=1){
			HashMapNode* hnm = (HashMapNode*) hn;
			delete(hnm->super.key);
			hnm->super.key = NULL;
			if(hnm->super.value){
			    delete(hnm->super.value);
			    hnm->super.value = NULL;
			}
			delete_HashMapNodeList(hn->next);
			hn->next = NULL;
			if(getAllocated(hn)){
				delete(hn);
			}
			hnm = NULL;
			hn = NULL;
		  }
		  else{
			 releaseReference(hn);
		  }
	   }
}

struct _HASH* combine_hashesHashMap(struct _HASH* h1, struct _HASH* h2){
	return NULL;

}
struct _Hashnode* remove_from_HashMap(struct _HASH* h, void* key){
	return NULL;
}
void *copy_HashMap(void* hmap){
    HashMap* map;
    if(hmap){
	   map = (HashMap*) hmap;
	   return map->super.vtable.copy(hmap);
    }
    return NULL;
}
void* copy_HashMapNode(void* iNode){
    HashMapNode* node_ptr, *node;
    node = (HashMapNode*) iNode;
    node_ptr = new(OBJECTIVE,sizeof(HashMapNode));
    init_HashMapNode((struct _Hashnode*)node_ptr,copy(node->super.key),copy(node->super.value),node->super.vtable.toStringKey,node->super.vtable.toStringValue);
    node_ptr->super.vtable = node->super.vtable;
    node_ptr->super.next = node->super.next;
    return node_ptr;
}
