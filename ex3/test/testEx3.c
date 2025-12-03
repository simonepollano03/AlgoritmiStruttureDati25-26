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

static void test_pq_pop(void) {
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

static void test_pq_empty_operations(void) {
    pq = priority_queue_create(int_compare, int_hash);
    TEST_ASSERT_NULL(priority_queue_top(pq));

    priority_queue_pop(pq);
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(pq));
}

static void test_pq_null_operations(void) {
    TEST_ASSERT_EQUAL_INT(-1, priority_queue_push(NULL, NULL));
    TEST_ASSERT_EQUAL_INT(0, priority_queue_contains(NULL, NULL));
    TEST_ASSERT_NULL(priority_queue_top(NULL));
    TEST_ASSERT_EQUAL_INT(0, priority_queue_size(NULL));
    TEST_ASSERT_TRUE(priority_queue_is_empty(NULL));

    priority_queue_pop(NULL);
    priority_queue_free(NULL);
}

static void test_pq_ternary_structure(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[20];
    for (int i = 0; i < 20; i++) {
        values[i] = i + 1;
        priority_queue_push(pq, &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(20, priority_queue_size(pq));

    int prev = 21;
    while (!priority_queue_is_empty(pq)) {
        int* top = (int*)priority_queue_top(pq);
        TEST_ASSERT_TRUE(*top < prev);
        prev = *top;
        priority_queue_top(pq);
    }
}  

static void test_pq_duplicates(void) {
    pq = priority_queue_create(int_compare, int_hash);

    int values[] = {5, 5, 5, 10, 10, 20, 20};
    int n = 7;

    for (int i = 0; i < n; i++) {
        priority_queue_push(pq, &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(n, priority_queue_size(pq));

    int* top = (int*)priority_queue_top(pq);
    TEST_ASSERT_EQUAL_INT(20, *top);
}

// ==================== TEST SCHEDULER ====================

static void test_task_functions(void) {
    Task t1 = {1, 0, 10, 5};
    Task t2 = {2, 0, 10, 10};

    TEST_ASSERT_TRUE(task_compare_priority(&t1, &t2) < 0);
    TEST_ASSERT_TRUE(task_compare_priority(&t2, &t1) > 0);

    TEST_ASSERT_TRUE(task_hash(&t1) != task_hash(&t2));

    TEST_ASSERT_EQUAL_INT(0, task_compare_equal(&t1, &t2));
    TEST_ASSERT_TRUE(task_compare_equal(&t1, &t2) != 0);
}

static void test_scheduler_simple(void) {
    Task tasks[] = {          // Id=_ , Start=_ , Length=_, Priorirty=_
        {1, 0, 5, 1},
        {2, 0, 3, 10},
        {3, 0, 2, 5}
    };
    
    TaskResult* results = NULL;
    int executed = 0;

    int ret = run_scheduler(tasks, 3, &results, &executed);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_NOT_NULL(results);
    TEST_ASSERT_EQUAL_INT(3, executed);

    TEST_ASSERT_EQUAL_INT(2, results[0].id);
    TEST_ASSERT_EQUAL_INT(0, results[0].start_time);
    TEST_ASSERT_EQUAL_INT(3, results[0].end_time);
    
    TEST_ASSERT_EQUAL_INT(3, results[1].id);
    TEST_ASSERT_EQUAL_INT(3, results[1].start_time);
    TEST_ASSERT_EQUAL_INT(5, results[1].end_time);
    
    TEST_ASSERT_EQUAL_INT(1, results[2].id);
    TEST_ASSERT_EQUAL_INT(5, results[2].start_time);
    TEST_ASSERT_EQUAL_INT(10, results[2].end_time);

    free(results);
}

static void test_scheduler_with_arrivals(void) {
    Task tasks[] = {          // Id=_ , Start=_ , Length=_, Priorirty=_
        {1, 0, 5, 1},
        {2, 3, 2, 10},
        {3, 10, 1, 5}
    };

    TaskResult* results = NULL;
    int executed = 0;

    int ret = run_scheduler(tasks, 3, &results, &executed);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_NOT_NULL(results);
    TEST_ASSERT_EQUAL_INT(3, executed);

    // Task 1 inizia subito (unico disponibile a t=0)
    TEST_ASSERT_EQUAL_INT(1, results[0].id);
    TEST_ASSERT_EQUAL_INT(0, results[0].start_time);
    TEST_ASSERT_EQUAL_INT(5, results[0].end_time);
    
    // A t=5, task 2 è già arrivato (a t=3) ed è l'unico
    TEST_ASSERT_EQUAL_INT(2, results[1].id);
    TEST_ASSERT_EQUAL_INT(5, results[1].start_time);
    TEST_ASSERT_EQUAL_INT(7, results[1].end_time);
    
    // Task 3 arriva a t=10, dopo che l'esecutore è libero a t=7
    TEST_ASSERT_EQUAL_INT(3, results[2].id);
    TEST_ASSERT_EQUAL_INT(10, results[2].start_time);
    TEST_ASSERT_EQUAL_INT(11, results[2].end_time);

    free(results);
}

static void test_scheduler_idle_time(void) {
    Task tasks[] = {
        {1, 0, 2, 5},
        {2, 10, 3, 10}
    };

    TaskResult* results = NULL;
    int executed = 0;

    int ret = run_scheduler(tasks, 2, &results, &executed);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_NOT_NULL(results);
    TEST_ASSERT_EQUAL_INT(2, executed);

    TEST_ASSERT_EQUAL_INT(1,results[0].id);
    TEST_ASSERT_EQUAL_INT(0, results[0].start_time);

    TEST_ASSERT_EQUAL_INT(2, results[1].id);
    TEST_ASSERT_EQUAL_INT(10,results[1].start_time);
    TEST_ASSERT_EQUAL_INT(13, results[1].end_time);

    free(results);
}

static void test_scheduler_null_params(void) {
    Task tasks[] = {{1, 0, 5, 1}};
    TaskResult* results = NULL;
    int count = 0;

    TEST_ASSERT_EQUAL_INT(-1, run_scheduler(NULL, 1, &results, &count));
    TEST_ASSERT_EQUAL_INT(-1, run_scheduler(tasks, 0, &results, &count));
    TEST_ASSERT_EQUAL_INT(-1, run_scheduler(tasks, 1, NULL, &count));
    TEST_ASSERT_EQUAL_INT(-1, run_scheduler(NULL, 1, &results, NULL));
}

static void test_scheduler_single_task(void) {
    Task tasks[] = {{1, 5, 10, 1}};
    TaskResult* results = NULL;
    int executed = 0;

    int ret = run_scheduler(tasks, 1, &results, &executed);
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_NOT_NULL(results);
    TEST_ASSERT_EQUAL_INT(1, executed);

    TEST_ASSERT_EQUAL_INT(1, results[0].id);
    TEST_ASSERT_EQUAL_INT(5, results[0].start_time);
    TEST_ASSERT_EQUAL_INT(15, results[0].end_time);

    free(results);
}

int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   TEST - ESERCIZIO 3                   ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    UNITY_BEGIN();

    printf("--- TEST PRIORITY QUEUE: CREAZIONE ---\n");
    RUN_TEST(test_pq_create);
    RUN_TEST(test_pq_create_null_params);
    printf("\n");

    printf("--- TEST PRIORITY QUEUE: PUSH ---\n");
    RUN_TEST(test_pq_push_single);
    RUN_TEST(test_pq_push_multiple);
    printf("\n");

    printf("--- TEST PRIORITY QUEUE: POP ---\n");
    RUN_TEST(test_pq_pop);
    RUN_TEST(test_pq_pop_all_sorted);
    printf("\n");

    printf("--- TEST PRIORITY QUEUE: CONTAINS ---\n");
    RUN_TEST(test_pq_contains);
    printf("\n");

    printf("--- TEST PRIORITY QUEUE: REMOVE ---\n");
    RUN_TEST(test_pq_remove);
    RUN_TEST(test_pq_remove_not_found);
    printf("\n");

    printf("--- TEST PRIORITY QUEUE: EDGE CASES ---\n");
    RUN_TEST(test_pq_empty_operations);
    RUN_TEST(test_pq_null_operations);
    RUN_TEST(test_pq_ternary_structure);
    RUN_TEST(test_pq_duplicates);
    printf("\n");

    printf("--- TEST SCHEDULER: FUNZIONI BASE ---\n");
    RUN_TEST(test_task_functions);
    printf("\n");

    printf("--- TEST SCHEDULER: SIMULAZIONE ---\n");
    RUN_TEST(test_scheduler_simple);
    RUN_TEST(test_scheduler_with_arrivals);
    RUN_TEST(test_scheduler_idle_time);
    RUN_TEST(test_scheduler_single_task);
    RUN_TEST(test_scheduler_null_params);
    printf("\n");

    return UNITY_END();
}