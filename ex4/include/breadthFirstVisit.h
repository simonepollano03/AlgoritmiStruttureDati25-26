#ifndef BREADTH_FIRST_VISIT_H
#define BREADTH_FIRST_VISIT_H
    #include "graph.h"

    /**
     * @file breadth_first_visit.h
     * @brief Implementazione dell'algoritmo di visita in ampiezza (BFS)
     * 
     * Fornisce funzionalità per esplorare un grafo attraverso 
     * l'algoritmo Breadth-First Search (BFS).
     */

    /**
     * @brief Esegue una visita in ampiezza (BFS) del grafo a partire da un nodo
     * 
     * L'algoritmo esplora il grafo livello per livello, visitando prima tutti
     * i nodi adiacenti al nodo di partenza, poi i nodi adiacenti a questi, e così via.
     * 
     * @param gr Grafo da visitare
     * @param start Nodo di partenza per la visita
     * @param compare Funzione di confronto tra nodi (ritorna 0 se uguali)
     * @param hash Funzione di hash per i nodi
     * @return void** Array di puntatori ai nodi nell'ordine di visita, 
     *                terminato da NULL. Ritorna NULL se il nodo start non è 
     *                presente nel grafo o in caso di errore.
     *                L'array deve essere liberato dal chiamante.
     * 
     * @note La funzione utilizza internamente una coda e una hash table per 
     *       tenere traccia dei nodi visitati
     * @note Se il grafo contiene più componenti connesse, vengono visitati 
     *       solo i nodi raggiungibili da start
     * 
     * @example
     * @code
     * Graph g = graph_create(0, 0, string_compare, string_hash);
     * graph_add_node(g, "A");
     * graph_add_node(g, "B");
     * graph_add_edge(g, "A", "B", NULL);
     * 
     * void** visited = breadth_first_visit(g, "A", string_compare, string_hash);
     * // visited conterrà: ["A", "B", NULL]
     * 
     * free(visited);
     * graph_free(g);
     * @endcode
     */
    void** breadth_first_visit(Graph gr,
                            void* start,
                            int (*compare)(const void*, const void*),
                            unsigned long (*hash)(const void*));

#endif