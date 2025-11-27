#include <stdlib.h>
#include <string.h>
    #include "../include/priorityQueue.h"
    #include "../../ex2/include/hashTable.h"

#define INITIAL_CAPACITY 16     /** @brief Capacità iniziale dell'array heap */

/**
 * @brief Struttura interna della Priority Queue
 * 
 * Utilizza un array dinamico per rappresentare l'heap ternario e
 * una hash table per mappare elementi -> posizioni (per contains/remove in O(1))
 */
struct priority_queue {
    void** heap;                                    /**< Array dinamico per l'heap */
    size_t size;                                    /**< Numero di elementi correnti */
    size_t capacity;                                /**< Capacità dell'array */
    int (*compare)(const void*, const void*);       /**< Funzione di confronto priorità */
    unsigned long (*hash)(const void*);             /**< Funzione di hash */
    HashTable* positions;                           /**< Mappa elemento -> indice */
};

/*============= FUNZIONI HELPER =================*/

/**
 * @brief Calcola l'indice del padre in un heap ternario
 * @param i Indice del nodo figlio
 * @return Indice del padre
 */
static inline size_t parent_index(size_t i) {
    return (i - 1) / 3;
}

/**
 * @brief Calcola l'indice del primo figlio in un heap ternario
 * @param i Indice del nodo padre
 * @return Indice del primo figlio
 */
static inline size_t first_child_index(size_t i) {
    return 3 * i + 1;
}

/**
 * @brief Scambia due elementi nell'heap e aggiorna la hash table
 */
static void swap_elements(PriorityQueue* pq, size_t i, size_t j) {
    // Scambia nell'array
    void* temp = pq->heap[i];
    pq->heap[i] = pq->heap[j];
    pq->heap[j] = temp;

    /* Aggiorna le posizioni nella hash table
       Creiamo wrapper per gli indici da salvare nella hash table */
    size_t* pos_i = (size_t*)malloc(sizeof(size_t));
    size_t* pos_j = (size_t*)malloc(sizeof(size_t));

    if (pos_i != NULL && pos_j != NULL) {
        *pos_i = i;
        *pos_j = j;

        // Rimuovi vecchie posizioni e inserisci nuove
        hash_table_put(pq->positions, pq->heap[i], pos_i);
        hash_table_put(pq->positions, pq->heap[j], pos_j);
    }
}

//....

/*============= FUNZIONI PUBBLICHE =================*/
PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {

}

int priority_queue_push(PriorityQueue* pq, void* element) {

}

int priority_queue_contains(const PriorityQueue* pq, const void* element) {

}

void* priority_queue_top(const PriorityQueue* pq) {

}

void priority_queue_pop(PriorityQueue* pq) {

}

int priority_queue_remove(PriorityQueue* pq, const void* element) {

}

int priority_queue_size(const PriorityQueue* pq) {

}

int priority_queue_is_empty(const PriorityQueue* pq) {

}

void priority_queue_free(PriorityQueue* pq) {

}

