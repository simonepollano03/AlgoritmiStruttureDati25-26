#include <stdio.h>
#include <string.h>
#include <stdlib.h>
    #include "../include/hashTable.h"
    #include "unity/unity.h"

static HashTable *table;

static int int_compare(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;

    return val_a -val_b;
}

unsigned long int_hash(const void* key) {
    return (unsigned long)(*(int*)key);
}

static int string_compare(const void* a, const void* b) {
    return strcmp((const char *)a, (const char *)b);
}

static unsigned long string_hash(const void *key) {
    const char *str = (const char *)key;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void setUp(void) {           // Rinominata per Unity
    table = NULL;
}

void tearDown(void) {        // Rinominata per Unity
    if (table != NULL) {
        hash_table_free(table);
        table = NULL;
    }
}

static void test_hash_table_create(void) {
    table = hash_table_create(string_compare, string_hash);
    TEST_ASSERT_NOT_NULL(table);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));
}

static void test_hash_table_create_null_params(void) {
    table = hash_table_create(NULL, string_hash);
    TEST_ASSERT_NULL(table);

    table = hash_table_create(string_compare, NULL);
    TEST_ASSERT_NULL(table);
}

static void test_hash_table_put_single(void) {
    table = hash_table_create(string_compare, string_hash);

    char *key = "test";
    int value = 42;

    hash_table_put(table, key, &value);

    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));

    int *result = (int *)hash_table_get(table, key);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_INT(42, *result);
}

static void test_hash_table_put_multiple(void) {
    table = hash_table_create(string_compare, string_hash);

    char *keys[] = {"uno", "due", "tre", "quattro", "cinque"};
    int values[] = {1, 2, 3, 4, 5};
    int n_values = 5;

    for (int i = 0; i < n_values; i++) {
        hash_table_put(table, keys[i], &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(n_values, hash_table_size(table));

    for (int i = 0; i < n_values; i++) {
        int *result = (int *)hash_table_get(table, keys[i]);
        TEST_ASSERT_NOT_NULL(result);
        TEST_ASSERT_EQUAL_INT(values[i], *result);
    }
}

static void test_hash_table_put_update(void) {
    table = hash_table_create(string_compare, string_hash);
    
    char *key = "test";
    int value1 = 10;
    int value2 = 20;

    hash_table_put(table, key, &value1);
    hash_table_put(table, key, &value2);

    TEST_ASSERT_EQUAL_INT(1, hash_table_size(table));

    int *result = (int *)hash_table_get(table, key);
    TEST_ASSERT_EQUAL_INT(20, *result);
}

static void test_hash_table_get_not_found(void) {
    table = hash_table_create(string_compare, string_hash);
    
    char *key = "test";
    int value = 42;

    hash_table_put(table, key, &value);

    void *result = hash_table_get(table, "inesistente");
    TEST_ASSERT_NULL(result);
}

static void test_hash_table_contains_key() {
    table = hash_table_create(string_compare, string_hash);
    
    char *key = "test";
    int value = 42;

    TEST_ASSERT_FALSE(hash_table_contains_key(table, key));

    hash_table_put(table, key, &value);

    TEST_ASSERT_TRUE(hash_table_contains_key(table, key));
    TEST_ASSERT_FALSE(hash_table_contains_key(table, "inesistente"));
}

static void test_hash_table_remove(void) {    
    table = hash_table_create(string_compare, string_hash);
    
    char *key = "test";
    int value = 42;

    hash_table_put(table, key, &value);

    hash_table_remove(table, key);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));
    TEST_ASSERT_NULL(hash_table_get(table, key));
}

static void test_hash_table_remove_with_collisions(void) {
    table = hash_table_create(string_compare, string_hash);
    
    char *keys[] = {"a", "b", "c"};
    int values[] = {1, 2, 3};
    int n_values = 3;

    for (int i = 0; i < n_values; i++) {
        hash_table_put(table, keys[i], &values[i]);
    }
    
    hash_table_remove(table, "b");

    TEST_ASSERT_EQUAL_INT(2, hash_table_size(table));
    TEST_ASSERT_TRUE(hash_table_contains_key(table, "a"));
    TEST_ASSERT_FALSE(hash_table_contains_key(table, "b"));
    TEST_ASSERT_TRUE(hash_table_contains_key(table, "c"));
}

static void test_hash_table_size(void) {
    table = hash_table_create(string_compare, string_hash);
    
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));

    char *keys[] = {"a", "b", "c", "d", "e"};
    int values[] = {1, 2, 3, 4, 5};
    int n_values = 5;

    for (int i = 0; i < n_values; i++) {
        hash_table_put(table, keys[i], &values[i]);
        TEST_ASSERT_EQUAL_INT(i+1, hash_table_size(table));

    }
    
    hash_table_remove(table, "c");
    TEST_ASSERT_EQUAL_INT(4, hash_table_size(table));
}    

static void test_hash_table_keyset(void) {
    table = hash_table_create(string_compare, string_hash);
    
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));

    char *keys[] = {"uno", "due", "tre"};
    int values[] = {1, 2, 3};
    int n_values = 3;

    for (int i = 0; i < n_values; i++) {
        hash_table_put(table, keys[i], &values[i]);
    }
    
    void **keyset = hash_table_keyset(table);
    TEST_ASSERT_NOT_NULL(keyset);

    int found_count = 0;
    for (int i = 0; i < n_values; i++) {
        for (int j = 0; j < n_values; j++) {
            if (strcmp((char *)keyset[i], keys[j]) == 0) {
                found_count++;
                break;
            }
        }
    }

    TEST_ASSERT_EQUAL_INT(n_values, found_count);

    free(keyset);
}


static void test_hash_table_keyset_empty(void) {
    table = hash_table_create(string_compare, string_hash);
    
    void **keyset = hash_table_keyset(table);
    TEST_ASSERT_NULL(keyset);
}

static void test_hash_table_with_integers(void) {
    table = hash_table_create(string_compare, string_hash);
    
    int keys[] = {10, 20, 30};
    char *values[] = {"dieci", "venti", "trenta"};
    int n_values = 3;

    for (int i = 0; i < n_values; i++) {
        hash_table_put(table, &keys[i], values[i]);
    }
    
    TEST_ASSERT_EQUAL_INT(n_values, hash_table_size(table));

    char *result = (char *)hash_table_get(table, &keys[1]);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("venti", result);
}

static void test_hash_table_massive_insert(void) {
    table = hash_table_create(int_compare, int_hash);

    int n_values = 1000;
    int *keys = (int *)malloc(n_values * sizeof(int));
    int *values = (int *)malloc(n_values * sizeof(int));

    for (int i = 0; i < n_values; i++) {
        keys[i] = i;
        values[i] = i * 10;
        hash_table_put(table, &keys[i], &values[i]);
    }

    TEST_ASSERT_EQUAL_INT(n_values, hash_table_size(table));

    for (int i = 0; i < n_values; i += 100) {
        int *result = (int *)hash_table_get(table, &keys[i]);
        TEST_ASSERT_NOT_NULL(result);
        TEST_ASSERT_EQUAL_INT(i * 10, *result);
    }

    free(keys);
    free(values);
}

static void test_hash_table_null_operations(void) {
    table = hash_table_create(string_compare, string_hash);

    hash_table_put(NULL, "test", NULL);
    hash_table_put(table, NULL, NULL);

    void *result = hash_table_get(NULL, "test");
    TEST_ASSERT_NULL(result);

    result = hash_table_get(table, NULL);
    TEST_ASSERT_NULL(result);

    TEST_ASSERT_FALSE(hash_table_contains_key(NULL, "test"));
    TEST_ASSERT_FALSE(hash_table_contains_key(table, NULL));

    hash_table_remove(NULL, "test");
    hash_table_remove(table, NULL);
    TEST_ASSERT_EQUAL_INT(0, hash_table_size(table));
}


int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   TEST - ESERCIZIO 2                   ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    UNITY_BEGIN();

    // Test creazione
    printf("--- TEST CREAZIONE HASHTABLE ---\n");
    RUN_TEST(test_hash_table_create);
    RUN_TEST(test_hash_table_create_null_params);
    printf("\n");

    // Test inserimento
     printf("--- TEST INSERIMENTO ---\n");
    RUN_TEST(test_hash_table_put_single);
    RUN_TEST(test_hash_table_put_multiple);
    RUN_TEST(test_hash_table_put_update);
    printf("\n");

    // Test get
    printf("--- TEST GET ---\n");
    RUN_TEST(test_hash_table_get_not_found);
    RUN_TEST(test_hash_table_contains_key);
    printf("\n");

    // Test rimozione
    printf("--- TEST RIMOZIONE ---\n");
    RUN_TEST(test_hash_table_remove);
    RUN_TEST(test_hash_table_remove_with_collisions);
    printf("\n");

    // Test size e keyset
    printf("--- TEST SIZE E KEYSET ---\n");
    RUN_TEST(test_hash_table_size);
    RUN_TEST(test_hash_table_keyset);
    RUN_TEST(test_hash_table_keyset_empty);
    printf("\n");

    // Test con diversi tipi di dato
    printf("--- TEST TIPI DI DATO ---\n");
    RUN_TEST(test_hash_table_with_integers);
    printf("\n");

    // Test stress
    printf("--- TEST STRESS E RESIZE ---\n");
    RUN_TEST(test_hash_table_massive_insert);
    printf("\n");

    // Test casi limite
    printf("--- TEST CASI LIMITE ---\n");
    RUN_TEST(test_hash_table_null_operations);
    printf("\n");

    return UNITY_END();
}