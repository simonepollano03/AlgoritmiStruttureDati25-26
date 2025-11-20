#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
    #include "../include/breadthFirstVisit.h"
    #include "../include/csvParser.h"
    #include "unity/unity.h"

static Graph graph;

// Funzioni utility per test
static int string_compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

static unsigned long string_hash(const void* key) {
    const char* str = (const char*)key;
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
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

    TEST_ASSERT_EQUAL_INT(node_size, graph_num_nodes(graph));

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
static void test_graph_add_edge_directed(void) {
    graph = graph_create(0, 1, string_compare, string_hash);

    char* node1 = "A";
    char* node2 = "B";

    graph_add_node(graph, node1);
    graph_add_node(graph, node2);

    TEST_ASSERT_EQUAL_INT(1, graph_add_edge(graph, node1, node2, NULL));
    TEST_ASSERT_EQUAL_INT(1, graph_num_edges(graph));
    TEST_ASSERT_TRUE(graph_contains_edge(graph, node1, node2));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, node2, node1));
}

static void test_graph_add_edge_undirected(void) {
     graph = graph_create(0, 0, string_compare, string_hash);

    char* node1 = "A";
    char* node2 = "B";

    graph_add_node(graph, node1);
    graph_add_node(graph, node2);

    TEST_ASSERT_EQUAL_INT(1, graph_add_edge(graph, node1, node2, NULL));
    TEST_ASSERT_EQUAL_INT(1, graph_num_edges(graph));
    TEST_ASSERT_TRUE(graph_contains_edge(graph, node1, node2));
    TEST_ASSERT_TRUE(graph_contains_edge(graph, node2, node1));
}

static void test_graph_add_edge_labelled(void) {
    graph = graph_create(1, 0, string_compare, string_hash);

    char* node1 = "A";
    char* node2 = "B";
    float distance = 10.5f;

    graph_add_node(graph, node1);
    graph_add_node(graph, node2);

    TEST_ASSERT_EQUAL_INT(1, graph_add_edge(graph, node1, node2, &distance));

    float* label = (float*)graph_get_label(graph, node1, node2);
    TEST_ASSERT_NOT_NULL(label);
    TEST_ASSERT_EQUAL_FLOAT(10.5f, *label);
}

static void test_graph_add_duplicate_edge(void) {
    graph = graph_create(0, 1, string_compare, string_hash);

    char* node1 = "A";
    char* node2 = "B";

    graph_add_node(graph, node1);
    graph_add_node(graph, node2);

    TEST_ASSERT_EQUAL_INT(1, graph_add_edge(graph, node1, node2, NULL));
    TEST_ASSERT_EQUAL_INT(0, graph_add_edge(graph, node1, node2, NULL));
    TEST_ASSERT_EQUAL_INT(1, graph_num_edges(graph));
}

static void test_graph_add_edge_nonexistent_nodes(void) {
    graph = graph_create(0, 1, string_compare, string_hash);
    
    TEST_ASSERT_EQUAL_INT(-1, graph_add_edge(graph, "A", "B", NULL));
    
    graph_add_node(graph, "A");
    TEST_ASSERT_EQUAL_INT(-1, graph_add_edge(graph, "A", "B", NULL));
}

static void test_graph_remove_edge(void) {
    graph = graph_create(0, 1, string_compare, string_hash);
    
    char* node1 = "A";
    char* node2 = "B";
    
    graph_add_node(graph, node1);
    graph_add_node(graph, node2);
    graph_add_edge(graph, node1, node2, NULL);
    
    TEST_ASSERT_EQUAL_INT(1, graph_remove_edge(graph, node1, node2));
    TEST_ASSERT_EQUAL_INT(0, graph_num_edges(graph));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, node1, node2));
}

static void test_graph_remove_edge_undirected(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* node1 = "A";
    char* node2 = "B";
    
    graph_add_node(graph, node1);
    graph_add_node(graph, node2);
    graph_add_edge(graph, node1, node2, NULL);
    
    TEST_ASSERT_EQUAL_INT(1, graph_remove_edge(graph, node1, node2));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, node1, node2));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, node2, node1));
}

static void test_graph_get_neighbours(void) {
    graph = graph_create(0, 1, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C", "D"};
    
    for (int i = 0; i < 4; i++) {
        graph_add_node(graph, nodes[i]);
    }
    
    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "A", "C", NULL);
    graph_add_edge(graph, "A", "D", NULL);
    
    void** neighbours = graph_get_neighbours(graph, "A");
    TEST_ASSERT_NOT_NULL(neighbours);
    
    int num = graph_num_neighbours(graph, "A");
    TEST_ASSERT_EQUAL_INT(3, num);
    
    free(neighbours);
}

static void test_graph_num_neighbours(void) {
    graph = graph_create(0, 1, string_compare, string_hash);
    
    graph_add_node(graph, "A");
    graph_add_node(graph, "B");
    
    TEST_ASSERT_EQUAL_INT(0, graph_num_neighbours(graph, "A"));
    
    graph_add_edge(graph, "A", "B", NULL);
    TEST_ASSERT_EQUAL_INT(1, graph_num_neighbours(graph, "A"));
}

static void test_graph_remove_node_with_edges(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C"};
    
    for (int i = 0; i < 3; i++) {
        graph_add_node(graph, nodes[i]);
    }
    
    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "A", "C", NULL);
    
    TEST_ASSERT_EQUAL_INT(1, graph_remove_node(graph, "A"));
    TEST_ASSERT_EQUAL_INT(2, graph_num_nodes(graph));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, "B", "A"));
    TEST_ASSERT_FALSE(graph_contains_edge(graph, "C", "A"));
}

// TEST BFV

static void test_bfs_simple_path(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C", "D"};
    
    for (int i = 0; i < 4; i++) {
        graph_add_node(graph, nodes[i]);
    }
    
    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "B", "C", NULL);
    graph_add_edge(graph, "C", "D", NULL);
    
    void** visited = breadth_first_visit(graph, "A", string_compare, string_hash);
    
    TEST_ASSERT_NOT_NULL(visited);
    TEST_ASSERT_EQUAL_STRING("A", (char*)visited[0]);
    TEST_ASSERT_EQUAL_STRING("B", (char*)visited[1]);
    TEST_ASSERT_EQUAL_STRING("C", (char*)visited[2]);
    TEST_ASSERT_EQUAL_STRING("D", (char*)visited[3]);
    TEST_ASSERT_NULL(visited[4]);
    
    free(visited);
}

static void test_bfs_tree(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C", "D", "E"};
    
    for (int i = 0; i < 5; i++) {
        graph_add_node(graph, nodes[i]);
    }
    
    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "A", "C", NULL);
    graph_add_edge(graph, "B", "D", NULL);
    graph_add_edge(graph, "B", "E", NULL);
    
    void** visited = breadth_first_visit(graph, "A", string_compare, string_hash);
    
    TEST_ASSERT_NOT_NULL(visited);
    TEST_ASSERT_EQUAL_STRING("A", (char*)visited[0]);
    // B e C sono al livello 1 (ordine può variare)
    // D e E sono al livello 2 (ordine può variare)
    TEST_ASSERT_NULL(visited[5]);
    
    free(visited);
}

static void test_bfs_single_node(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    graph_add_node(graph, "A");
    
    void** visited = breadth_first_visit(graph, "A", string_compare, string_hash);
    
    TEST_ASSERT_NOT_NULL(visited);
    TEST_ASSERT_EQUAL_STRING("A", (char*)visited[0]);
    TEST_ASSERT_NULL(visited[1]);
    
    free(visited);
}

static void test_bfs_disconnected_graph(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    char* nodes[] = {"A", "B", "C", "D"};
    
    for (int i = 0; i < 4; i++) {
        graph_add_node(graph, nodes[i]);
    }
    
    graph_add_edge(graph, "A", "B", NULL);
    graph_add_edge(graph, "C", "D", NULL);
    
    void** visited = breadth_first_visit(graph, "A", string_compare, string_hash);
    
    TEST_ASSERT_NOT_NULL(visited);
    TEST_ASSERT_EQUAL_STRING("A", (char*)visited[0]);
    TEST_ASSERT_EQUAL_STRING("B", (char*)visited[1]);
    TEST_ASSERT_NULL(visited[2]);
    
    free(visited);
}

static void test_bfs_nonexistent_start(void) {
    graph = graph_create(0, 0, string_compare, string_hash);
    
    graph_add_node(graph, "A");
    
    void** visited = breadth_first_visit(graph, "B", string_compare, string_hash);
    TEST_ASSERT_NULL(visited);
}

// TEST CSV PARSER

static void test_csv_record_operations(void) {
    CSVRecord record;
    record.place1 = (char*)malloc(10);
    record.place2 = (char*)malloc(10);
    strcpy(record.place1, "Milano");
    strcpy(record.place2, "Torino");
    record.distance = 125.5f;
    
    TEST_ASSERT_EQUAL_STRING("Milano", record.place1);
    TEST_ASSERT_EQUAL_STRING("Torino", record.place2);
    TEST_ASSERT_EQUAL_FLOAT(125.5f, record.distance);
    
    free_csv_record(&record);
}

int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   TEST - ESERCIZIO 4                   ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    UNITY_BEGIN();

    // Test creazione grafo
    printf("--- TEST CREAZIONE GRAFO ---\n");
    RUN_TEST(test_graph_create);
    RUN_TEST(test_graph_create_undirected_unlabelled);
    RUN_TEST(test_graph_create_null_params);
    printf("\n");

    // Test gestione nodi
    printf("--- TEST GESTIONE NODI ---\n");
    RUN_TEST(test_graph_add_node);
    RUN_TEST(test_graph_add_multiple_nodes);
    RUN_TEST(test_graph_add_duplicate_node);
    RUN_TEST(test_graph_contains_node);
    RUN_TEST(test_graph_remove_node);
    RUN_TEST(test_graph_remove_nonexistent_node);
    RUN_TEST(test_graph_get_nodes);
    printf("\n");

    // Test gestione archi
    printf("--- TEST GESTIONE ARCHI ---\n");
    RUN_TEST(test_graph_add_edge_directed);
    RUN_TEST(test_graph_add_edge_undirected);
    RUN_TEST(test_graph_add_edge_labelled);
    RUN_TEST(test_graph_add_duplicate_edge);
    RUN_TEST(test_graph_add_edge_nonexistent_nodes);
    RUN_TEST(test_graph_remove_edge);
    RUN_TEST(test_graph_remove_edge_undirected);
    RUN_TEST(test_graph_get_neighbours);
    RUN_TEST(test_graph_num_neighbours);
    RUN_TEST(test_graph_remove_node_with_edges);
    printf("\n");

    // Test BFV
    printf("--- TEST BFV ---\n");
    RUN_TEST(test_bfs_simple_path);
    RUN_TEST(test_bfs_tree);
    RUN_TEST(test_bfs_single_node);
    RUN_TEST(test_bfs_disconnected_graph);
    RUN_TEST(test_bfs_nonexistent_start);
    printf("\n");

    // Test csv parser
    printf("--- TEST CSV PARSER ---\n");
    RUN_TEST(test_csv_record_operations);
    printf("\n");
    
        
    return UNITY_END();
}