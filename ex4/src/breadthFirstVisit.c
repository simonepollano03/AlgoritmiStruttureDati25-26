#include <stdio.h>
#include <stdlib.h>
    #include "../include/breadthFirstVisit.h"
    #include "../../ex2/include/hashTable.h"

// Struttura per coda fifo
typedef struct queue_node {
    void* data;
    struct queue_node* next;
} QueueNode; 

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

static Queue* queue_create(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    queue->front = NULL;
    queue->rear  = NULL;
    queue->size  = 0;
}

static int queue_is_empty(const Queue* queue) {
    return queue == NULL || queue->front == NULL;
}

// enqueue si intede che inserisce un elemento al fondo alla coda
static int queue_enqueue(Queue* queue, void* data) {
    if (queue == NULL) return 0;

    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) return 0;

    new_node->data = data;
    new_node->next = NULL;

    if (queue_is_empty(queue)) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node; 
    }

    queue->size++;

    return 1;
}


// dequeue si intede che inserisce un elemento all'inizio alla coda
static int queue_dequeue(Queue* queue) {
    if (queue_is_empty(queue)) return NULL;

    QueueNode* temp = queue->front;
    void* data = temp->data;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;

    return data;
}

static void queue_free(Queue* queue) {
    if (queue == NULL) return;

    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }

    free(queue);
}

void** breadth_first_visit(Graph gr, void* start,
                            int (*compare)(const void*, const void*),
                            unsigned long (*hash)(const void*)) {
                                
    if (gr == NULL || start == NULL || 
        compare == NULL || hash == NULL) return NULL;
    
    
    // Verifico che il nodo di partenza esista
    if (!graph_contains_node(gr, start)) return NULL;

    // Creo l'hash table per tenere traccia dei nodi visitati
    HashTable* visited = hash_table_create(compare, hash);
    if (visited == NULL) return NULL;

    // Creo la coda per BFV
    Queue* queue = queue_create();
    if (queue == NULL) {
        hash_table_free(visited);
        return NULL;
    }

    // Creo array dinamico per memorizzare l'ordine di visita
    int capacity = 100;
    int count    = 0;

    void** result = (void**)malloc(capacity * sizeof(void*));
    if (result == NULL) {
        queue_free(queue);
        hash_table_free(visited);
        return NULL;
    }

    // Marco il nodo iniziale come visitato e lo aggiungo alla coda
    int* visited_market = (int*)malloc(sizeof(int));
    if (visited_market == NULL) {
        free(result);
        queue_free(queue);
        hash_table_free(visited);
        return NULL;
    }
    *visited_market = 1;

    hash_table_put(visited, start, visited_market);
    queue_enqueue(queue, start);

    // BFV principale
    while (!queue_is_empty(queue)) {
        void* current = queue_dequeue(queue);

        // Aggiungo il nodo corrente al risultato
        if (count >= capacity) {
            capacity *= 2;
            
            void** new_result = (void**)realloc(result, capacity * sizeof(void*));
            if (new_result == NULL) {
                free(result);
                queue_free(queue);
                hash_table_free(visited);
                return NULL;
            }
            result = new_result;
        }
        result[count++] = current;
    
        // Ottengo i vicini del nodo corrente
        void** neighbours = graph_get_neighbours(gr, current);
        if (neighbours != NULL) {
            int num_neighbours = graph_num_neighbours(gr, current);
        
            for (int i = 0; i < num_neighbours; i++) {
                void* neighbour = neighbours[i];

                // Se non l'ho visitato, lo marco e lo aggiungo alla coda
                if (!hash_table_contains_key(visited, neighbour)) {
                    int* marker = (int*)malloc(sizeof(int));
                    if (marker == NULL) {
                        *marker = 1;
                        hash_table_put(visited, neighbour, marker);
                        queue_enqueue(queue, neighbour);
                    }
                }
            }
        
            free(neighbours);
        }  
    }

    // Aggiungo NULL come terminatore
    if (count >= capacity) {
        void** new_result = (void**)realloc(result, (count + 1) * sizeof(void*));
        if (new_result == NULL) {
            free(result);
            queue_free(queue);
            hash_table_free(visited);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;

    // Libero tutte le strutture temp che ho creato 
    void** keys = hash_table_keyset(visited);
    if (keys != NULL) {
        int size = hash_table_size(visited);

        for (int i = 0; i < size; i++) {
            void* marker = hash_table_get(visited, keys[i]);
            if (marker != NULL) {
                free(marker);
            } 
        }

        free(keys);
    }

    hash_table_free(visited);
    queue_free(queue);

    return result;
}
