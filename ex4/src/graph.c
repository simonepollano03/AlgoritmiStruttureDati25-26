#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/graph.h"
    #include "../../ex2/include/hashTable.h"

// Lista di adiacenza per un nodo
typedef struct adjacency_node {
    void* dest;                     /**< Nodo destinazione */
    void* label;                    /**< Etichetta dell'arco */
    struct adjacency_node* next;    /**< Prossimo nodo nella lista */
} AdjacencyNode;

// Lista di adiacenza (lista concatenata di nodi adiacenti)
typedef struct {
    AdjacencyNode* head;
    int size;
} AdjacencyList;

// structura interna del grafo
struct graph {
    int labelled;                                   /**< 1 se etichettato, 0 altrimenti */
    int directed;                                   /**< 1 se diretto, 0 altrimenti */
    HashTable* adjacency_map;                       /**< Mappa nodo -> lista di adiacenza */
    int num_nodes;                                  /**< Numero di nodi */
    int num_edges;                                  /**< Numero di archi */
    int (*compare)(const void*, const void*);       /**< Funzione di confronto */
    unsigned long (*hash)(const void*);             /**< Funzione di hash */
};

static AdjacencyList* adjacency_list_create(void) {
    AdjacencyList* list = (AdjacencyList*)malloc(sizeof(AdjacencyList));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->size = 0;

    return list;
}

static AdjacencyList* adjacency_list_find(AdjacencyList* list, const void* dest,
                                          int (*compare)(const void*, const void*)) {

    if (list == NULL || dest == NULL) return NULL;

    AdjacencyNode* current = list->head;
    while(current != NULL) {
        if (compare(current->dest, dest) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;    
}

static int adjancency_list_add(AdjacencyList* list, const void* dest, const void* label) {
    if (list == NULL || dest == NULL) return -1;

    AdjacencyNode* new_node = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    if (new_node == NULL) return -1;

    new_node->dest  = (void*)dest;
    new_node->label = (void*)label;
    new_node->next  = list->head;
    
    list->head = new_node;
    list->size++;

    return 1;
}

static int adjancency_list_remove(AdjacencyList* list, const void* dest,
                                  int (*compare)(const void*, const void*)) { 
    
    if (list == NULL || dest == NULL) return 0;
    
    AdjacencyNode* current = list->head;
    AdjacencyNode* prev = NULL;

    while (current != NULL) {
        if (compare(current->dest, dest) == 0) {
            if (prev == NULL) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            list->size--;
            return 1;
        }

        prev = current;
        current = current->next;
    }

    return 0;
}


static void adjancency_list_remove(AdjacencyList* list) {
    if (list == NULL) return;

    AdjacencyNode* current = list->head;
    while (current != NULL) {
        AdjacencyNode* next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}


// ===== Funzioni principali ===== //

Graph graph_create(int labelled, int directed,
                   int (*compare)(const void*, const void*),
                   unsigned long (*hash)(const void*)) {
    
//    if
}

int graph_is_directed(const Graph gr) {

}

int graph_is_labelled(const Graph gr) {

}


int graph_add_node(Graph gr, const void* node) {

}


int graph_contains_node(const Graph gr, const void* node) {

}


int graph_remove_node(Graph gr, const void* node) {

}

int graph_num_nodes(const Graph gr) {

}

void** graph_get_nodes(const Graph gr) {

}

int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {

}

int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {

}

int graph_remove_edge(Graph gr, const void* node1, const void* node2) {

}

int graph_num_edges(const Graph gr) {

}

Edge** graph_get_edges(const Graph gr) {

}

void* graph_get_label(const Graph gr, const void* node1, const void* node2) {

}

void** graph_get_neighbours(const Graph gr, const void* node) {

}

int graph_num_neighbours(const Graph gr, const void* node) {

}

void graph_free(Graph gr) {

}
