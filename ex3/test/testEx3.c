#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/priorityQueue.h"
    #include "../include/scheduler.h"
    #include "unity/unity.h"


static PriorityQueue* pq = NULL;

void setUp(void) {
    pq = NULL;
}

void tearDown(void) {
    if (pq != NULL) {
        priority_queue_free(pq);
        pq = NULL;
    }
}

// ==================== FUNZIONI HELPER TEST ====================

/* Confronto per interi (max-heap) */
static int int_compare(const void* a, const void* b) {
    int val_a = *(const int*)a;
    int val_b = *(const int*)b;
    return val_a - val_b;
}

/* Hash per interi */
static unsigned long int_hash(const void* element) {
    return (unsigned long)(*(const int*)element);
}

// ==================== TEST PRIORITY QUEUE ====================

static void test_pq_create(void) {
    pq = priority_queue_create(int_compare, int_hash);
    TEST_ASSERT_NOT_NULL(pq);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
    TEST_ASSERT_TRUE(priority_queue_is_empty(pq));
}

static void test_pq_create_null_params(void) {
    PriorityQueue* pq_null = priority_queue_create(NULL, int_hash);
    TEST_ASSERT_NULL(pq_null);

    pq_null = priority_queue_create(int_compare, NULL);
    TEST_ASSERT_NULL(pq_null);
}

static void test_pq_push_single(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int value = 42;
    int result = priority_queue_push(pq, &value);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, priority_queue_size(pq));
    TEST_ASSERT_FALSE(priority_queue_is_empty(pq));

    int* top = (int*)priority_queue_top(pq);
    TEST_ASSERT_NOT_NULL(top);
    TEST_ASSERT_EQUAL_INT(42, *top);
}

static void test_pq_push_multiple(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 3, 6, 11, 15, 1};
    int n = 7;

    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(n, priority_queue_size(pq));

    int* top = (int*)priority_queue_top(pq);
    TEST_ASSERT_NOT_NULL(top);
    TEST_ASSERT_EQUAL_INT(15, *top);
}

static void test_pop(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 3};
    int n = 3;

    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(2, priority_queue_size(pq));

    int* top = (int*)priority_queue_top(pq);
    TEST_ASSERT_NOT_NULL(top);
    TEST_ASSERT_EQUAL_INT(5, *top);

}

static void test_pq_pop_all_sorted(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 3, 8, 1, 15, 7};
    int n = 7;
    int expected[] = {15, 10, 8, 7, 5, 3, 1};

    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    for (int i = 0; i < n; i++) {
        int* top = (int*)priority_queue_top(pq);
        TEST_ASSERT_NOT_NULL(top);
        TEST_ASSERT_EQUAL_INT(expected[i], *top);
        priority_queue_pop(pq);
    }

    TEST_ASSERT_TRUE(priority_queue_is_empty(pq));
}

static void test_pq_contains(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 9};
    int n = 3;
    
    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    TEST_ASSERT_TRUE(priority_queue_contains(pq, &values[0]));
    TEST_ASSERT_TRUE(priority_queue_contains(pq, &values[1]));
    TEST_ASSERT_TRUE(priority_queue_contains(pq, &values[2]));

    int not_in = 25;
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &not_in));
}

static void test_pq_remove(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 9};
    int n = 3;
    
    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    int result = priority_queue_remove(pq, &values[0]);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(n, priority_queue_size(pq));
    TEST_ASSERT_FALSE(priority_queue_contains(pq, &values[0]));

    int* top = (int*)priority_queue_top(pq);
    TEST_ASSERT_EQUAL_IT(10, *top);
}

static void test_pq_remove_not_found(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 10, 9};
    int n = 3;
    
    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    int not_in = 99;
    int result = priority_queue_remove(pq, &not_in);
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_EQUAL_INT(n, priority_queue_size(pq));
}


// ==================== FUNZIONI HELPER TEST ====================
