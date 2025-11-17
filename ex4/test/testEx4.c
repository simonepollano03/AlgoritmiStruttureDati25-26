#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
    #include "../include/breadthFirstVisit.h"
    #include "../include/csvParser.h"
    #include "unity/unity.h"

static Graph graph;

// Funzioni utility per test
static int int_compare(const void* a, const void* b) {
    int val_a = *(int*)a;
    int val_b = *(int*)b;
    return val_a - val_b; 
}

static int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

static unsigned long int_hash(const void* key) {
    return (unsigned long)(*(int*)key);
}

static unsigned long string_hash(const void* key) {
    const char* str = (const char*)key;
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void setUp(void) {
    graph = NULL;
}

void tearDown(void) {
    if (graph != NULL) {
        graph_free(graph);
        graph = NULL;
    }
}

// TEST CREAZIONE GRAFO
static void test_graph_create(void) {
    graph = graph_create(1, 1, string_compare, string_hash);

    TEST_ASSERT_NOT_NULL(graph);
    TEST_ASSERT_EQUAL_INT(1, graph_is_labelled(graph));
    TEST_ASSERT_EQUAL_INT(1, graph_is_directed(graph));
    TEST_ASSERT_EQUAL_INT(0, graph_num_nodes(graph));
    TEST_ASSERT_EQUAL_INT(0, graph_num_edges(graph));
}

static void test_graph_create_undirected_unlabelled(void) {
    graph = graph_create(0, 0, string_compare, string_hash);

    TEST_ASSERT_NOT_NULL(graph);
    TEST_ASSERT_EQUAL_INT(0, graph_is_labelled(graph));
    TEST_ASSERT_EQUAL_INT(0, graph_is_directed(graph));
}

static void test_graph_create_null_params(void) {
    graph = graph_create(1, 1, NULL, string_hash);
    TEST_ASSERT_NOT_NULL(graph);

    graph = graph_create(1, 1, string_compare, NULL);
    TEST_ASSERT_NOT_NULL(graph);
}

static void test_graph_add_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* node = "A";
    int result = graph_add_node(graph, node);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, graph_num_nodes(graph));
    TEST_ASSERT_TRUE(graph_contains_node(graph, node));
}

static void test_graph_add_multiple_nodes(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C", "D", "E"};
    int node_size = 5;

    for (int i = 0; i < node_size; i++) {
        TEST_ASSERT_EQUAL_INT(1, graph_add_node(graph, nodes[i]));
    }

    TEST_ASSERT_EQUAL_INT(node_size, graph_num_node(graph));

    for (int i = 0; i < node_size; i++) {
        TEST_ASSERT_TRUE(graph_contains_node(graph, nodes[i]));
    }
}

static void test_graph_add_duplicate_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* node = "A";
    
    TEST_ASSERT_EQUAL_INT(1, graph_add_node(graph, node));
    TEST_ASSERT_EQUAL_INT(0, graph_add_node(graph, node));
    TEST_ASSERT_EQUAL_INT(1, graph_num_nodes(graph));
}

static void test_graph_contains_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* node = "A";
    TEST_ASSERT_FALSE(graph_contains_node(graph, node));

    graph_add_node(graph, node);
    TEST_ASSERT_TRUE(graph_contains_node(graph, node));
    TEST_ASSERT_FALSE(graph_contains_node(graph, "B"));
}

static void test_graph_remove_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* node = "A";
    graph_add_node(graph, node);
    
    TEST_ASSERT_EQUAL_INT(1, graph_remove_node(graph, node));
    TEST_ASSERT_EQUAL_INT(0, graph_num_nodes(graph));
    TEST_ASSERT_FALSE(graph_contains_node(graph, node));
}

static void test_graph_remove_nonexistent_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    TEST_ASSERT_EQUAL_INT(0, graph_remove_node(graph, "A"));
}

static void test_graph_get_nodes(void) {
    graph = graph_create(0, 0, string_compare, string_hash);

    char* nodes[] = {"A", "B", "C"};
    int node_size = 3;

    for (int i = 0; i < node_size; i++) {
        graph_add_node(graph, nodes[i]);
    }

    void** retrieved = graph_get_nodes(graph);
    TEST_ASSERT_NOT_NULL(retrieved);

    int found = 0;
    for (int i = 0; i < node_size; i++) {
        for (int j = 0; j < node_size; j++) {
            if (string_compare(retrieved[i], nodes[j]) == 0) {
                found++;
                break;
            }
        }
    }

    TEST_ASSERT_EQUAL_INT(node_size, found);
    free(retrieved);
}

// TEST ARCHI

//...


// TEST BFV

//...

// TEST CSV PARSER

//...


int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   TEST - ESERCIZIO 4                   ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    UNITY_BEGIN();

    // Test creazione grafo
    RUN_TEST(test_graph_create);
    RUN_TEST(test_graph_create_undirected_unlabelled);
    RUN_TEST(test_graph_create_null_params);
    RUN_TEST(test_graph_add_node);
    RUN_TEST(test_graph_add_multiple_nodes);
    RUN_TEST(test_graph_add_duplicate_node);
    RUN_TEST(test_graph_contains_node);
    RUN_TEST(test_graph_remove_node);
    RUN_TEST(test_graph_remove_nonexistent_node);
    RUN_TEST(test_graph_get_nodes);

    // Test archi

    // Test BFV

    // Test csv parser
        
    return UNITY_END();
}