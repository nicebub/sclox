/*
//  Test_HashMap.c
//  Test_HashMap
//
//  Created by Scott Lorberbaum on 3/15/20.
//  Copyright © 2020 Scott Lorberbaum. All rights reserved.
//*/
#include "unity.h"
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "HashMap.h"
#include "memory.h"
static HashMap* m;
int *r;
char* print_int(void* a){
    char * n;
    n = NULL;
    asprintf(&n,"%d",*(int*)a);
/*    fprintf(stderr,"found int : %s\n",n);*/
    return n;
}
void setUp(void){
    m = NULL;
    m = (HashMap*)create_hashmap(3,&print_int);
    r = NULL;
    r = new(RAW,sizeof(int));

}
void tearDown(void){
    delete_hashmap((struct _HASH*)m);
    m = NULL;
}

void test_create_hashmap(void){
    *r =1 ;
    int i;
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    for(i=0;i<m->super.used;i++);
	   TEST_ASSERT_EQUAL_PTR(NULL,m->super.Buckets[i]);
}

void test_delete_hashmap(void){
    delete_hashmap((struct _HASH*)m);
    m = NULL;
    tearDown();
    setUp();
    delete_hash((struct _HASH*)m);
    m = NULL;
    tearDown();
    setUp();
}

void test_computer_hash_value(void){
    char* string1, *string2, *string3;
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    TEST_ASSERT_EQUAL_INT(1,m->super.vtable.compute_hash_value((struct _HASH*)m,strdup("no")));
    TEST_ASSERT_EQUAL_INT(-1,m->super.vtable.compute_hash_value((struct _HASH*)m,NULL));
    TEST_ASSERT_EQUAL_INT(-1,m->super.vtable.compute_hash_value(NULL,NULL));
    TEST_ASSERT_EQUAL_INT(-1,m->super.vtable.compute_hash_value(NULL,strdup("hey")));
}
void test_get_value_for_key(void){
    int* x;
    char* string1;
    x = new(RAW,sizeof(int));
    *x = 1;
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key(NULL,"no"));
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key(NULL,NULL));
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key((struct _HASH*)m,NULL));
    TEST_ASSERT_EQUAL_PTR(NULL,(int*)get_value_for_keymap((struct _HASH*)m, "no"));
    TEST_ASSERT_EQUAL_PTR(NULL,(int*)get_value_for_key(&m->super, "no"));
    string1 = new(RAW,sizeof(char)*(strlen("hello")+1));
    memset(string1,0,strlen("hello")+1);
    strcpy(string1,"hello");
    add_to_hash((struct _HASH*)m,string1,x);
    TEST_ASSERT_EQUAL_INT(1,*(int*)get_value_for_key(&m->super,"hello"));
}
void test_add_to_hash(void){
    int * a,*b;
    char* string1, *string2;
    a = new(RAW,sizeof(int));
    *a = 2;
    b = new(RAW,sizeof(int));
   *b = 3;
    
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    add_to_hash((struct _HASH*)m,NULL,a);
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    add_to_hash((struct _HASH*)m,NULL,NULL);
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    string1 = new(RAW,sizeof(char)*(strlen("a")+1));
    memset(string1,0,strlen("a")+1);
    strcpy(string1,"a");
    add_to_hash(NULL,string1,a);
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    string1 = new(RAW,sizeof(char)*(strlen("hello")+1));
    memset(string1,0,strlen("hello")+1);
    strcpy(string1,"hello");
    add_to_hash((struct _HASH*)m,string1,a);
    TEST_ASSERT_EQUAL_INT(1,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    add_to_hash((struct _HASH*)m,string1,b);
    TEST_ASSERT_EQUAL_INT(1,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = new(RAW,sizeof(int));
    *b = 7;
    string2 = new(RAW,sizeof(char)*(strlen("nothere")+1));
    memset(string2,0,strlen("nothere")+1);
    strcpy(string2,"nothere");
    add_to_hash((struct _HASH*)m,string2,b);
    TEST_ASSERT_EQUAL_INT(2,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = new(RAW,sizeof(int));
    *b = 4;
    string2 = new(RAW,sizeof(char)*(strlen("nothereagain")+1));
    memset(string2,0,strlen("nothereagain")+1);
    strcpy(string2,"nothereagain");
    add_to_hashmap((struct _HASH*)m,string2,b);
    TEST_ASSERT_EQUAL_INT(3,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = new(RAW,sizeof(int));
    *b = 9;
    string2 = new(RAW,sizeof(char)*(strlen("nothe  again")+1));
    memset(string2,0,strlen("nothe  again")+1);
    strcpy(string2,"nothe  again");
    add_to_hashmap((struct _HASH*)m,string2,b);
    TEST_ASSERT_EQUAL_INT(4,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);

}

void test_toString(void){
    char * temp,*string1;
//    TEST_ASSERT_EQUAL_STRING("no",)
    r = new(RAW,sizeof(int));
    *r = 1;
    string1 = new(RAW,sizeof(char)*(strlen("no")+1));
    memset(string1,0,strlen("no")+1);
    strcpy(string1,"no");
    add_to_hash((struct _HASH*)m,string1,r);
    r = new(RAW,sizeof(int));
    *r = 2;
    string1 = new(RAW,sizeof(char)*(strlen("hello")+1));
    memset(string1,0,strlen("hello")+1);
    strcpy(string1,"hello");
    add_to_hash((struct _HASH*)m,string1,r);
    r = new(RAW,sizeof(int));
    *r = 3;
    string1 = new(RAW,sizeof(char)*(strlen("good")+1));
    memset(string1,0,strlen("good")+1);
    strcpy(string1,"good");
    add_to_hashmap((struct _HASH*)m,string1,r);
    temp = (char*)m->super.vtable.toString((struct _HASH*)m);
    TEST_ASSERT_EQUAL_STRING("((hello,2),(good,3),(no,1))",temp);
    delete(temp);
    temp = NULL;
}

void test_print_hash(void){
    char* string1;
    r = new(RAW,sizeof(int));
    *r = 14;
    string1 = new(RAW,sizeof(char)*(strlen("onestring")+1));
    memset(string1,0,strlen("onestring")+1);
    strcpy(string1,"onestring");
    add_to_hashmap((struct _HASH*)m,string1,r);
    r = new(RAW,sizeof(int));
    *r = 17;
    string1 = new(RAW,sizeof(char)*(strlen("twostring")+1));
    memset(string1,0,strlen("twostring")+1);
    strcpy(string1,"twostring");
    add_to_hashmap((struct _HASH*)m,string1,r);
    print_hashmap((struct _HASH*)m);
    print_hash((struct _HASH*)m);
}
int main(int argc, const char**argv){
    UNITY_BEGIN();
    RUN_TEST(test_create_hashmap);
    RUN_TEST(test_delete_hashmap);
    RUN_TEST(test_computer_hash_value);
    RUN_TEST(test_add_to_hash);
    RUN_TEST(test_get_value_for_key);
    RUN_TEST(test_toString);
    RUN_TEST(test_print_hash);
    return UNITY_END();
}
