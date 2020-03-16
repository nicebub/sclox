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
static HashMap* m;
int *r;
void setUp(void){
    m = NULL;
    m = (HashMap*)create_hashmap(3);
    r = NULL;
    r = malloc(sizeof(int));

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
    x = malloc(sizeof(int));
    *x = 1;
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key(NULL,"no"));
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key(NULL,NULL));
    TEST_ASSERT_EQUAL_PTR(NULL,get_value_for_key((struct _HASH*)m,NULL));
    TEST_ASSERT_EQUAL_PTR(NULL,(int*)get_value_for_keymap((struct _HASH*)m, "no"));
    TEST_ASSERT_EQUAL_PTR(NULL,(int*)get_value_for_key(&m->super, "no"));
    add_to_hash((struct _HASH*)m,strdup("hello"),x);
    TEST_ASSERT_EQUAL_INT(1,*(int*)get_value_for_key(&m->super,"hello"));
}
void test_add_to_hash(void){
    int * a,*b;
    a = malloc(sizeof(int));
    *a = 2;
    b = malloc(sizeof(int));
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
    add_to_hash(NULL,strdup("a"),a);
    TEST_ASSERT_EQUAL_INT(0,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    add_to_hash((struct _HASH*)m,strdup("hello"),a);
    TEST_ASSERT_EQUAL_INT(1,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    add_to_hash((struct _HASH*)m,strdup("hello"),b);
    TEST_ASSERT_EQUAL_INT(1,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = malloc(sizeof(int));
    *b = 7;
    add_to_hash((struct _HASH*)m,strdup("nothere"),b);
    TEST_ASSERT_EQUAL_INT(2,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = malloc(sizeof(int));
    *b = 4;
    add_to_hashmap((struct _HASH*)m,strdup("nothereagain"),b);
    TEST_ASSERT_EQUAL_INT(3,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);
    b = malloc(sizeof(int));
    *b = 9;
    add_to_hashmap((struct _HASH*)m,strdup("nothe  again"),b);
    TEST_ASSERT_EQUAL_INT(3,m->super.used);
    TEST_ASSERT_EQUAL_INT(3,m->super.size);
    TEST_ASSERT_EQUAL_INT(2,m->super.alpha);

}

void test_toString(void){
    char * temp;
//    TEST_ASSERT_EQUAL_STRING("no",)
    r = malloc(sizeof(int));
    *r = 1;
    add_to_hash((struct _HASH*)m,strdup("no"),r);
    r = malloc(sizeof(int));
    *r = 2;
    add_to_hash((struct _HASH*)m,strdup("hello"),r);
    r = malloc(sizeof(int));
    *r = 3;
    add_to_hashmap((struct _HASH*)m,strdup("good"),r);
    temp = (char*)m->super.vtable.toString((struct _HASH*)m);
    TEST_ASSERT_EQUAL_STRING("((hello,2),(good,3),(no,1))",temp);
    free(temp);
    temp = NULL;
}

void test_print_hash(void){
    r = malloc(sizeof(int));
    *r = 14;
    add_to_hashmap((struct _HASH*)m,strdup("onestring"),r);
    r = malloc(sizeof(int));
    *r = 17;
    add_to_hashmap((struct _HASH*)m,strdup("twostring"),r);
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
