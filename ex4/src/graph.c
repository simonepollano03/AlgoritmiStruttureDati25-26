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

static AdjacencyNode* adjacency_list_find(AdjacencyList* list, const void* dest,
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

static int adjacency_list_add(AdjacencyList* list, const void* dest, const void* label) {
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

static int adjacency_list_remove(AdjacencyList* list, const void* dest,
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


static void adjacency_list_free(AdjacencyList* list) {
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
    hash_table_put(gr->adjacency_map, node, list);
    gr->num_nodes++;

    return 1;
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
    void** nodes = hash_table_keyset(gr->adjacency_map);
    if (nodes == NULL) return -1;

    int size = hash_table_size(gr->adjacency_map);

    // Rimuovo gli archi che puntano al nodo
    for (int i = 0; i < size; i++) {
        if (gr->compare(nodes[i], node) != 0) {
            AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, nodes[i]);
            if (list != NULL) {
                if (adjacency_list_remove(list, node, gr->compare)) {
                    gr->num_edges--;
                }
            }
        }
    }

    // Rimuovo gli archi uscenti dal nodo
    AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, node);
    if (list != NULL) {
        gr->num_edges -= list->size;
        adjacency_list_free(list);
    }

    // Rimuovo il nodo dalla mappa
    hash_table_remove(gr->adjacency_map, node);
    gr->num_nodes--;

    free(nodes);
    return 1;
}

int graph_num_nodes(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->num_nodes; 
}

void** graph_get_nodes(const Graph gr) {
    if (gr == NULL) return NULL;
    return hash_table_keyset(gr->adjacency_map);
}  

int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return -1;

    // Verifico che entrambi i nodi (node1, node2) esistano
    if (!(graph_contains_node(gr, node1) || graph_contains_node(gr, node2))) return -1;

    // Se il grafo non è etichettato, la label deve essere settata a NULL
    if (!gr->labelled && label != NULL) return -1;

    // Ottengo la lista di adiacenza del primo nodo
    AdjacencyList* list_node1 = (AdjacencyList*)hash_table_get(gr->adjacency_map, node1);
    if (list_node1 == NULL) return -1;

    // Verifico che l'arco esista già
    if (adjacency_list_find(list_node1, node2, gr->compare) != NULL) return 0;

    // Aggiungo l'arco nel caso in cui non esista
    if (adjacency_list_add(list_node1, node2, label) != 1) return -1;
    gr->num_edges++;

    // Se il grafo è non diretto, aggiungo l'arco inverso
    if (!gr->directed) {
        AdjacencyList* list_node2 = (AdjacencyList*)hash_table_get(gr->adjacency_map, node2);
        if (list_node2 == NULL) return -1;

        // Se l'aggiunta non va effettuo rollback all'aggiunta precedente
        if (adjacency_list_add(list_node2, node1, label) != 1) {
            adjacency_list_remove(list_node1, node2, gr->compare);
            gr->num_edges--;
            return -1;
        }
    }

    return 1;
}

int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return 0;

    AdjacencyList* list_node1 = (AdjacencyList*)hash_table_get(gr->adjacency_map, node1);
    if (list_node1 == NULL) return 0;

    return adjacency_list_find(list_node1, node2, gr->compare) != NULL ? 1 : 0;
}


int graph_remove_edge(Graph gr, const void* node1, const void* node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return -1;

    // Verifico che l'arco esista
    if (!graph_contains_edge(gr, node1, node2)) return 0;

    // Ottengo la lista di adiacenza del primo nodo
    AdjacencyList* list_node1 = (AdjacencyList*)hash_table_get(gr->adjacency_map, node1);
    if (list_node1 == NULL) return -1;

    // Rimuovo l'arco
    if (adjacency_list_remove(list_node1, node2, gr->compare)) 
        gr->num_edges--;

    // Se il grafo è non diretto, rimuovi anche l'arco inverso
    if (!gr->directed) {
        AdjacencyList* list_node2 = (AdjacencyList*)hash_table_get(gr->adjacency_map, node2);
        if (list_node2 != NULL) {
            adjacency_list_remove(list_node2, node1, gr->compare);
        }
    }

    return 1;
}

int graph_num_edges(const Graph gr) {
    if (gr == NULL) return 0;
    return gr->num_edges;
}

Edge** graph_get_edges(const Graph gr) {
    if (gr == NULL) return NULL;

    // Alloca array per gli archi
    Edge** edges = (Edge**)malloc(gr->num_edges * sizeof(Edge*));
    if (edges == NULL) return NULL;

    // Ottengo l'elenco di tutti i nodi del grafo
    void** nodes = hash_table_keyset(gr->adjacency_map);
    if (nodes == NULL) {
        free(edges);
        return NULL;
    }
    
    int edge_index = 0;
    int size = hash_table_size(gr->adjacency_map);

    // Scorro i nodi
    for (int i = 0; i < size; i++) {
        void* source = nodes[i];
        
        // Recupero la lista di adiacenza associata al nodo i-esimo
        AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, source);
        if (list != NULL) {
            AdjacencyNode* current = list->head;

            // Scorro tutti i nodi adiacenti, quindi gli archi in uscita
            while (current != NULL) {

                // Se il grafo è non diretto, evito di aggiungere lo stesso arco due volte 
                if (gr->directed || gr->compare(source, current->dest) < 0) {

                    // Alloco la memoria per un nunvo oggetto edge
                    Edge* edge = (Edge*)malloc(sizeof(Edge));
                    if (edge == NULL) {
                        for (int j = 0; j < edge_index; j++) {
                            free(edges[i]);
                        }
                        free(edges);
                        free(nodes);
                        return NULL;
                    }

                    // Riempio la struttura dell'arco
                    edge->source = source;
                    edge->dest = current->dest;
                    edge->label = current->label;
                    edges[edge_index++] = edge;
                }

                current = current->next;
            }
        }
    }

    free(nodes);
    return edges;
}

void* graph_get_label(const Graph gr, const void* node1, const void* node2) {
    if (gr == NULL || node1 == NULL || node2 == NULL ) return NULL;
    if (!gr->labelled) return NULL;

    AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, node1);
    if (list == NULL) return NULL;

    AdjacencyNode* node = adjacency_list_find(list, node2, gr->compare);
    if (node == NULL) return NULL;

    return node->label;
}

void** graph_get_neighbours(const Graph gr, const void* node) {
    if (gr == NULL || node == NULL) return NULL;

    AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, node);
    if (list == NULL || node == NULL) return NULL;

    void** neighbours = (void**)malloc(list->size * sizeof(void*));
    if (neighbours == NULL) return NULL;

    AdjacencyNode* current = list->head;
    int index = 0;

    while (current != 0) {
        neighbours[index++] = current->next;
        current = current->next;
    }

    return neighbours;
}

int graph_num_neighbours(const Graph gr, const void* node) {
    if (gr == NULL || node == NULL) return -1;

    AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, node);
    if (list == NULL) return 0;

    return list->size;
}

void graph_free(Graph gr) {
    if (gr == NULL) return;

    void** nodes = hash_table_keyset(gr->adjacency_map);
    if (nodes != NULL) {
        int size = hash_table_size(gr->adjacency_map);
        
        for (int i = 0; i < size; i++) {
            AdjacencyList* list = (AdjacencyList*)hash_table_get(gr->adjacency_map, nodes[i]);
            if (list != NULL) {
                adjacency_list_free(list);
            }
        }

        free(nodes);
    }
    
    hash_table_free(gr->adjacency_map);
    free(gr);
}
