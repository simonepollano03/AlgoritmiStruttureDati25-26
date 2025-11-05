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
    
    if (compare == NULL || hash == NULL) return NULL;

    struct graph* gr = (struct graph*)malloc(sizeof(struct graph));
    if (gr == NULL) return NULL;

    gr->adjacency_map = hash_table_create(compare, hash);
    if(gr->adjacency_map == NULL) {
        free(gr);
        return NULL;
    }

    gr->directed  = directed;
    gr->labelled  = labelled;
    gr->num_nodes = 0;
    gr->num_edges = 0;
    gr->compare   = compare;
    gr->hash      = hash;

    return gr;
}

int graph_is_directed(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->directed;
}

int graph_is_labelled(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->labelled;
}


int graph_add_node(Graph gr, const void* node) {
    if (gr == NULL || node == NULL) return -1;

    // Se il nodo esiste già ritorno 0
    if (hash_table_contains_key(gr->adjacency_map, node)) {
        return 0;
    }

    // Creo una nuova lista di adiacenza vuota
    AdjacencyList* list = adjacency_list_create();
    if (list == NULL) return -1;

    // Aggiungo il nodo alla mappa
    hash_table_put(gr->adjacency_map, nod, list);
    gr->num_nodes++;

    reutrn 1;
}


int graph_contains_node(const Graph gr, const void* node) {
    if (gr == NULL) return 0;

    return hash_table_contains_key(gr->adjacency_map, node);
}


int graph_remove_node(Graph gr, const void* node) {
    if (gr == NULL || node == NULL) return -1;

    // Verifico se il nodo esiste
    if (!graph_contains_node(gr, node)) return 0;

    // Ottengo tutti i suoi nodi
    void** nodes = hash_table_keyset(gr-<adjancency_map);
    if (nodes == NULL) return -1;

    int size = hash_table_size(gr->adjacency_map);

    // Rimuovo gli archi che puntano al nodo
    for (int i = 0; i < size; i++) {
        if (gr->compare(nodes[i], node) != 0) {

        }
    }

}

int graph_num_nodes(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->num_nodes; 
}

void** graph_get_nodes(const Graph gr) {
    if (gr == NULL) return NULL;

}  

int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return -1;
}

int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {
        if (gr == NULL || node1 == NULL || node2 == NULL ) return -1;

}

int graph_remove_edge(Graph gr, const void* node1, const void* node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return -1;

}

int graph_num_edges(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->num_edges;
}

Edge** graph_get_edges(const Graph gr) {
    if (gr == NULL) return NULL;

}

void* graph_get_label(const Graph gr, const void* node1, const void* node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return NULL;

}

void** graph_get_neighbours(const Graph gr, const void* node) {
        if (gr == NULL || node == NULL) return NULL;

}

int graph_num_neighbours(const Graph gr, const void* node) {
        if (gr == NULL || node == NULL) return NULL;

}

void graph_free(Graph gr) {
    if (gr == NULL) return;
}
