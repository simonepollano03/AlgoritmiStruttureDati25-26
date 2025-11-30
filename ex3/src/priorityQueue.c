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

/**
 * @brief Aggiorna la posizione di un elemento nella hash table
 */
static void update_position(PriorityQueue* pq, void* element, size_t index) {
    size_t* pos = (size_t*)malloc(sizeof(size_t));
    if (pos != NULL) {
        *pos = index;

        // Rimuovi vecchia posizione sse esiste e libera la memoria
        size_t* old_pos = (size_t*)hash_table_get(pq->positions, element);
        if (old_pos != NULL) {
            hash_table_remove(pq->positions, element);
            free(old_pos);
        }

        hash_table_put(pq->positions, element, pos);
    }
}


/**
 * @brief Bubble-up: fa risalire un elemento fino alla posizione corretta
 * 
 * Complessità: O(log3 n) 
 */
static void bubble_up(PriorityQueue* pq, size_t index) {
    while (index > 0) {
        size_t parent = parent_index(index);

        // Se il padre ha priorità >= figlio, OK
        if (pq->compare(pq->heap[parent], pq->heap[index]) >= 0) {
            break;
        }

        // se no scambia gli elementi e "sali"
        swap_elements(pq, index, parent);
        index = parent;
    }
}

/**
 * @brief Heapify-down: fa scendere un elemento fino alla posizione corretta
 * 
 * Complessità: O(log₃ n)
 */
static void heapify_down(PriorityQueue* pq, size_t index) {
    while (1) {
        size_t first_child = first_child_index(index);

        // Se non ha figli interrompi
        if (first_child >= pq->size) break;

        // Se si trova il figlio con priorità max tra i 3
        size_t max_child = first_child;

        for (size_t k = 1; k <= 2; k++) {
            size_t child = first_child + k;
            
            if (child < pq->size && pq->compare(pq->heap[child], pq->heap[max_child]) > 0) {
                max_child = child;
            }
        }

        // Se il padre ha priorità >= figlio max, OK
        if (pq->compare(pq->heap[index], pq->heap[max_child]) > 0) {
            break;
        }

        // se no scambia gli elementi e "scendi"
        swap_elements(pq, index, max_child);
        index = max_child;
    }
}

/**
 * @brief Espande la capacità dell'array heap
 */
static int resize_heap(PriorityQueue* pq) {
    size_t new_capacity = pq->capacity * 2;
    void** new_heap = (void**)realloc(pq->heap, new_capacity * sizeof(void*));
    
    if (new_heap == NULL) {
        return -1;
    }
    
    pq->heap = new_heap;
    pq->capacity = new_capacity;
    return 0;
}

/**
 * @brief Funzione di confronto per la hash table (confronta puntatori)
 */
static int pointer_compare(const void* a, const void* b) {
    return (a == b) ? 0 : 1;
}

/**
 * @brief Funzione di hash wrapper che usa la funzione fornita dall'utente
 */
static unsigned long (*user_hash_func)(const void*) = NULL;

static unsigned long pointer_hash(const void* ptr) {
    if (user_hash_func != NULL) {
        return user_hash_func(ptr);
    }
    /* Fallback: hash basato sull'indirizzo */
    return (unsigned long)(size_t)ptr;
}


/*============= FUNZIONI PUBBLICHE =================*/

PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {
    if (compare == NULL || hash == NULL) return NULL;

    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (pq == NULL) return NULL;

    pq->heap = (void**)malloc(INITIAL_CAPACITY * sizeof(void*));
    if (pq->heap == NULL) {
        free(pq);
        return NULL;
    }

    // Salva la funzione hash dell'utente per usarla nella hash table
    user_hash_func = hash;
    
    pq->positions = hash_table_create(pointer_compare, hash);
    if (pq->positions == NULL) {
        free(pq->heap);
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = INITIAL_CAPACITY;
    pq->compare = compare;
    pq->hash = hash;

    return pq;
}

int priority_queue_push(PriorityQueue* pq, void* element) {
    if (pq == NULL || element == NULL) return -1;

    // Espandi se necessario
    if (pq->size >= pq->capacity) {
        if (resize_heap(pq) != 0) return -1;
    }
    
    // Insersci elemento in fondo
    size_t index =pq->size;
    pq->heap[index] = element;
    pq->size++;

    // Aggiorna la posizione nella hash table
    update_position(pq, element, index);

    // Bubble up per rispristinare proprietà della heap
    bubble_up(pq, index);

    return 0;
}

int priority_queue_contains(const PriorityQueue* pq, const void* element) {
    if (pq == NULL || element == NULL) return 0;

    return hash_table_contains_key(pq->positions, element);
}

void* priority_queue_top(const PriorityQueue* pq) {
    if (pq == NULL || pq->size == 0) return;

    return pq->heap[0];
}

void priority_queue_pop(PriorityQueue* pq) {

}

int priority_queue_remove(PriorityQueue* pq, const void* element) {

}

int priority_queue_size(const PriorityQueue* pq) {
    if (pq == NULL) return 0;

    return (int)pq->size;
}

int priority_queue_is_empty(const PriorityQueue* pq) {
    if (pq == NULL) return -1;

    return pq->size == 0 ? 1 : 0;
}

void priority_queue_free(PriorityQueue* pq) {

}

