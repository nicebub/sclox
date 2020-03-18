/*
 * HashMap.c
 *
 *  Created on: Mar 15, 2020
 *      Author: scotty
 */

#include "HashMap.h"
#include "hash.h"
static struct _Hashnode_vtable HashNodeMap_vtable = {
		&delete_hashnodelmap,
		&set_hnode_valuemap,
		&set_hnode_keymap,
		&get_hnode_valuemap,
		&get_hnode_keymap,
		&get_hnode_value_typemap,
		&get_hnode_key_typemap,
	     &toStringMapNode,
	     &toStringMapNodeValue
};
static struct _hash_vtable hashmap_vtable = {
		&add_to_hashmap,
		&copy_hashmap,
		&get_value_for_keymap,
		&get_node_for_keymap,
		&compute_hash_valuemap,
		&combine_hashesmap,
		&print_hashmap,
		&delete_hashmap,
		&delete_hashnodemap,
		&remove_from_hashmap,
		&toStringMap

};

struct _HASH *create_hashmap(int size,char*(*func)(void* value)){

    HashMap *m;
    int i;
    HashMapNode** buck;
    m = malloc(sizeof(HashMap));
    m->super.vtable = hashmap_vtable;
    buck=malloc(sizeof(HashMapNode*)*size);
    m->super.Buckets = (struct _Hashnode**)buck;
    for(i =0;i<size;i++)
	   m->super.Buckets[i] = NULL;
    m->super.vtable.add_to_hash = &add_to_hashmap;
    m->super.vtable.combine_hashes = &combine_hashesmap;
    m->super.vtable.compute_hash_value = &compute_hash_valuemap;
    m->super.vtable.copy_hash = &copy_hashmap;
    m->super.vtable.delete_hash = &delete_hashmap;
    m->super.vtable.delete_hashnode = &delete_hashnodemap;
    m->super.vtable.get_value_for_key = &get_value_for_keymap;
    m->super.vtable.print_hash = &print_hashmap;
    m->super.vtable.remove_from_hash = &remove_from_hashmap;
    m->super.vtable.toString = &toStringMap;
    m->super.vtable.func = func;
    m->super.alpha = 2;
    m->super.size = size;
    m->super.used = 0;
    return (struct _HASH*)m;
}
void add_to_hashmap(struct _HASH* h, void * key,void * value){
/*    struct _hr r;*/
    short int res;
    HashMapNode *hn;
    HashMapNode *temp;
    HashMap *hm;
    if(h && key){

	   if(get_value_for_keymap(h,key)!=NULL){
		   HashMapNode* ex;
		   ex = (HashMapNode*)get_node_for_keymap(h,key);
		   free(ex->super.value);
		   ex->super.value = value;
		   return;
	   }
	   hm = (HashMap*)h;
	   hn = (HashMapNode*) create_hashnodemap(key,value,h->vtable.func );
	   res = compute_hash_valuemap(h,key);
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

HashMapNode* create_hashnodemap(void*key,void*value,char*(*func)(void* value) ){
	    HashMapNode *h;
	    h = malloc(sizeof(HashMapNode));
	    h->super.key = key;
	    h->super.value = value;
	   h->super.next = NULL;
	   h->super.vtable = HashNodeMap_vtable;
    h->super.vtable.func = func;

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
short int compute_hash_valuemap(struct _HASH * hashMap,void *key){
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


void * get_value_for_keymap(struct _HASH* h, void* key){

	char *n;
	short int res;
	struct _Hashnode *temp;

	if(h && key){
		res = compute_hash_valuemap(h,key);
		temp = ((HashMap*)h)->super.Buckets[res];
		n = (char*)key;
		while(temp != NULL){
			if(strcmp(temp->key,n)==0)
				return temp->value;
			temp = temp->next;
		}
	}
	return NULL;
}
struct _Hashnode * get_node_for_keymap(struct _HASH* h, void* key){

	char *n;
	short int res;
	struct _Hashnode *temp;

	if(h && key){
		res = compute_hash_valuemap(h,key);
		temp = ((HashMap*)h)->super.Buckets[res];
		n = (char*)key;
		while(temp != NULL){
			if(strcmp(temp->key,n)==0)
				return temp;
			temp = temp->next;
		}
	}
	return NULL;
}
char* toStringMapNodeValue(struct _Hashnode* node){
    if(node)
	   return node->vtable.func(node->value);
    return NULL;
}
char* toStringMapNode(struct _Hashnode* node){
    char* temp;
    char * num;
    size_t str_len;
    num = NULL;
    asprintf(&num,"%s",toStringMapNodeValue(node));
    str_len = (4+strlen(num)+strlen((char*)node->key));
    temp = malloc(sizeof(char)*str_len);
    memset(temp,0,str_len);
    strncat(temp,"(",1);
    strncat(temp,node->key,strlen(node->key));
    strncat(temp,",",1);
    strncat(temp,num,str_len);
    strncat(temp,")",1);
    free(num);
    return temp;
}
char* toStringMap(struct _HASH* h){
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
			 num = toStringMapNodeValue((struct _Hashnode*)node);
/*			 asprintf(&num,"%d",*(int*)node->super.value);*/
			 fullstr_len += strlen(node->super.key);
			 fullstr_len += strlen(num);
			 free(num);
			 num= NULL;
			 node= (HashMapNode*)node->super.next;
		  }
	   }
    }
    fullstr_len= (int)fullstr_len+1+(int)strlen(extra)+((int)strlen(each)*h->used+(h->used-1));
    temp = malloc(sizeof(char)*fullstr_len);
    memset(temp,0,fullstr_len);
    temp[0]='(';
    count = 0;
    for(i=0;i<h->size;i++){
	   HashMapNode* node = (HashMapNode*)((HashMap*)h)->super.Buckets[i];
	   if(node){
		  while(node){
			 char * nstr = toStringMapNode((struct _Hashnode*)node);
			 count++;
			 strncat(temp,nstr,fullstr_len-strlen(temp));
				if(node->super.next !=NULL)
				    strncat(temp,",",1);
			 node= (HashMapNode*)node->super.next;
			 free(nstr);
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
void delete_hashmap(struct _HASH* h){
    if(h){
		int t;
	   for(t=0;t<h->size;t++){
		  if(h->Buckets[t]){
			 HashMapNode* temp;
			 HashMapNode* next;
			 temp = (HashMapNode*)h->Buckets[t];
			 while(temp){
				next = (HashMapNode*)temp->super.next;
				delete_hashnodelmap((struct _Hashnode*)temp);
				temp = NULL;
				temp = next;
			 }
		  }
		  h->Buckets[t] = NULL;
	   }
/*	   for(a=1;a<=h->csize;a++){
		  free(h->cache[a-1].p);
		  h->cache[a-1].p = NULL;
	   }*/
/*	   free(h->cache);
	   h->cache = NULL;*/
	   free(h);
	   h = NULL;
    }

}
void print_hashmap(struct _HASH * h){
    fprintf(stderr,"%s\n",h->vtable.toString(h));
    
}
void delete_hashnodemap(struct _HASH* h,struct _Hashnode* hn){
    if(hn){
	   HashMapNode* hnm = (HashMapNode*) hn;
	   free(hnm->super.key);
	   hnm->super.key = NULL;
	   free(hnm->super.value);
	   hnm->super.value = NULL;
	   hnm->super.next = NULL;
	   free(hnm);
	   hnm = NULL;
	   hn = NULL;
    }
}

void delete_hashnodelmap(struct _Hashnode* hn){
	   if(hn){
		  HashMapNode* hnm = (HashMapNode*) hn;
		  free(hnm->super.key);
		  hnm->super.key = NULL;
		  if(hnm->super.value){
				free(hnm->super.value);
			 hnm->super.value = NULL;
		  }
    /*	   delete_hashnodel(hn->next);
		  hn->next = NULL;*/
		  free(hnm);
		  hnm = NULL;
		  hn = NULL;
	   }
}

void set_hnode_valuemap(struct _Hashnode* h, void * v){}
void set_hnode_keymap(struct _Hashnode*h, void * k){}
void * get_hnode_valuemap(struct _Hashnode* h){
	return NULL;
}
void * get_hnode_keymap(struct _Hashnode* h){
	return NULL;
}
int get_hnode_value_typemap(struct _Hashnode* hn){
	return -1;
}
int get_hnode_key_typemap(struct _Hashnode* hn){
	return -1;
}
struct _HASH* combine_hashesmap(struct _HASH* h1, struct _HASH* h2){
	return NULL;

}
struct _Hashnode* remove_from_hashmap(struct _HASH* h, void* key){
	return NULL;
}
struct _HASH *copy_hashmap(struct _HASH* h){
	return NULL;
}
