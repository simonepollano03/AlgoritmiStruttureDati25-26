#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
    
    /**
     * @brief Struttura opaca che rappresenta la Priority Queue
     */
    typedef struct priority_queue PriorityQueue;

    /**
     * @brief Crea una nuova Priority Queue vuota
     * 
     * @param compare Funzione di confronto tra elementi per determinare la priorità.
     *                Deve restituire:
     *                - valore > 0 se il primo elemento ha priorità maggiore
     *                - valore < 0 se il secondo elemento ha priorità maggiore
     *                - 0 se hanno la stessa priorità
     * @param hash Funzione di hash per gli elementi (usata internamente per
     *             la struttura di appoggio che permette contains/remove in O(log n))
     * @return PriorityQueue* Puntatore alla coda creata, NULL in caso di errore
     */
    PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*),
                                        unsigned long (*hash)(const void*));

    /**
     * @brief Inserisce un elemento nella Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue
     * @param element Elemento da inserire
     * @return int 0 in caso di successo, -1 in caso di errore
     * 
     * @note Complessità: O(log3 n)
     */
    int priority_queue_push(PriorityQueue* pq, void* element);

    /**
     * @brief Verifica se un elemento è presente nella Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue
     * @param element Elemento da cercare
     * @return int 1 se presente, 0 se non presente, -1 in caso di errore
     * 
     * @note Complessità: O(1) grazie alla hash table di appoggio
     */
    int priority_queue_contains(const PriorityQueue* pq, const void* element);

    /**
     * @brief Restituisce l'elemento con priorità massima senza rimuoverlo
     * 
     * @param pq Puntatore alla Priority Queue
     * @return void* Elemento con priorità massima, NULL se la coda è vuota o errore
     * 
     * @note Complessità: O(1)
     */
    void* priority_queue_top(const PriorityQueue* pq);

    /**
     * @brief Rimuove l'elemento con priorità massima dalla Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue
     * 
     * @note Complessità: O(log3 n)
     */
    void priority_queue_pop(PriorityQueue* pq);

    /**
     * @brief Rimuove un elemento specifico dalla Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue
     * @param element Elemento da rimuovere
     * @return int 0 in caso di successo, -1 se elemento non trovato o errore
     * 
     * @note Complessità: O(log3 n) grazie alla hash table di appoggio
     */
    int priority_queue_remove(PriorityQueue* pq, const void* element);

    /**
     * @brief Restituisce il numero di elementi nella Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue
     * @return int Numero di elementi, -1 in caso di errore
     * 
     * @note Complessità: O(1)
     */
    int priority_queue_size(const PriorityQueue* pq);

    /**
     * @brief Verifica se la Priority Queue è vuota
     * 
     * @param pq Puntatore alla Priority Queue
     * @return int 1 se vuota, 0 se non vuota, -1 in caso di errore
     * 
     * @note Complessità: O(1)
     */
    int priority_queue_is_empty(const PriorityQueue* pq);

    /**
     * @brief Libera la memoria allocata per la Priority Queue
     * 
     * @param pq Puntatore alla Priority Queue da liberare
     * 
     * @note Non libera la memoria degli elementi contenuti
     */
    void priority_queue_free(PriorityQueue* pq);
    
#endif