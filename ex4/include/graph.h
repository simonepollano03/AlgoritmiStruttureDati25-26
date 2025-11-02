#ifndef GRAPH_H
#define GRAPH_H
    #include <stdlib.h>

    /**
     * @file graph.h
     * @brief Libreria per la gestione di grafi sparsi
     * 
     * Implementa una struttura dati per grafi diretti/non diretti,
     * etichettati/non etichettati, ottimizzata per grafi sparsi.
     * Utilizza internamente una Hash Table per garantire accessi efficienti.
     */

    /**
     * @brief Struttura opaca che rappresenta un grafo
     */
    typedef struct graph *Graph;

    /**
     * @brief Struttura che rappresenta un arco del grafo
     */
    typedef struct edge {
        void* source;   /**< Nodo d'origine */
        void* dest;     /**< Nodo di destinazione */
        void* label;    /**< Etichetta dell'arco */
    } Edge;

    /**
     * @brief Crea un nuovo grafo vuoto
     * 
     * @param labelled 1 se il grafo è etichettato, 0 altrimenti
     * @param directed 1 se il grafo è diretto, 0 altrimenti
     * @param compare Funzione di confronto tra nodi (ritorna 0 se uguali)
     * @param hash Funzione di hash per i nodi
     * @return Graph Puntatore al grafo creato, NULL in caso di errore
     * 
     * @note Complessità: O(1)
     */
    Graph graph_create(int labelled,
                    int directed,
                    int (*compare)(const void*, const void*),
                    unsigned long (*hash)(const void*));

    /**
     * @brief Verifica se il grafo è diretto
     * 
     * @param gr Grafo da verificare
     * @return int 1 se diretto, 0 altrimenti
     * 
     * @note Complessità: O(1)
     */
    int graph_is_directed(const Graph gr);

    /**
     * @brief Verifica se il grafo è etichettato
     * 
     * @param gr Grafo da verificare
     * @return int 1 se etichettato, 0 altrimenti
     * 
     * @note Complessità: O(1)
     */
    int graph_is_labelled(const Graph gr);

    /**
     * @brief Aggiunge un nodo al grafo
     * 
     * @param gr Grafo
     * @param node Nodo da aggiungere
     * @return int 1 se aggiunto con successo, 0 se già presente, -1 in caso di errore
     * 
     * @note Complessità: O(1)
     */
    int graph_add_node(Graph gr, const void* node);

    /**
     * @brief Verifica se un nodo è presente nel grafo
     * 
     * @param gr Grafo
     * @param node Nodo da cercare
     * @return int 1 se presente, 0 altrimenti
     * 
     * @note Complessità: O(1)
     */
    int graph_contains_node(const Graph gr, const void* node);

    /**
     * @brief Rimuove un nodo dal grafo
     * 
     * Rimuove anche tutti gli archi incidenti al nodo
     * 
     * @param gr Grafo
     * @param node Nodo da rimuovere
     * @return int 1 se rimosso con successo, 0 se non presente, -1 in caso di errore
     * 
     * @note Complessità: O(N) dove N può essere il numero di nodi o archi
     */
    int graph_remove_node(Graph gr, const void* node);

    /**
     * @brief Restituisce il numero di nodi nel grafo
     * 
     * @param gr Grafo
     * @return int Numero di nodi
     * 
     * @note Complessità: O(1)
     */
    int graph_num_nodes(const Graph gr);

    /**
     * @brief Recupera tutti i nodi del grafo
     * 
     * @param gr Grafo
     * @return void** Array di puntatori ai nodi (deve essere liberato dal chiamante), NULL in caso di errore
     * 
     * @note Complessità: O(N) dove N è il numero di nodi
     */
    void** graph_get_nodes(const Graph gr);

    /**
     * @brief Aggiunge un arco al grafo
     * 
     * Se il grafo è non diretto, aggiunge automaticamente anche l'arco inverso.
     * Se il grafo è non etichettato, il parametro label deve essere NULL.
     * 
     * @param gr Grafo
     * @param node1 Nodo sorgente
     * @param node2 Nodo destinazione
     * @param label Etichetta dell'arco (può essere NULL se grafo non etichettato)
     * @return int 1 se aggiunto con successo, 0 se già presente, -1 in caso di errore
     * 
     * @note Complessità: O(1) assumendo hash table efficiente e grafo sparso
     */
    int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label);

    /**
     * @brief Verifica se un arco è presente nel grafo
     * 
     * @param gr Grafo
     * @param node1 Nodo sorgente
     * @param node2 Nodo destinazione
     * @return int 1 se presente, 0 altrimenti
     * 
     * @note Complessità: O(1) assumendo hash table efficiente e grafo sparso
     */
    int graph_contains_edge(const Graph gr, const void* node1, const void* node2);

    /**
     * @brief Rimuove un arco dal grafo
     * 
     * Se il grafo è non diretto, rimuove automaticamente anche l'arco inverso.
     * 
     * @param gr Grafo
     * @param node1 Nodo sorgente
     * @param node2 Nodo destinazione
     * @return int 1 se rimosso con successo, 0 se non presente, -1 in caso di errore
     * 
     * @note Complessità: O(1) assumendo hash table efficiente e grafo sparso
     */
    int graph_remove_edge(Graph gr, const void* node1, const void* node2);

    /**
     * @brief Restituisce il numero di archi nel grafo
     * 
     * @param gr Grafo
     * @return int Numero di archi
     * 
     * @note Complessità: O(N) dove N è il numero di nodi
     */
    int graph_num_edges(const Graph gr);

    /**
     * @brief Recupera tutti gli archi del grafo
     * 
     * @param gr Grafo
     * @return Edge** Array di archi (deve essere liberato dal chiamante), NULL in caso di errore
     * 
     * @note Complessità: O(N) dove N è il numero di archi
     */
    Edge** graph_get_edges(const Graph gr);

    /**
     * @brief Recupera l'etichetta di un arco
     * 
     * @param gr Grafo
     * @param node1 Nodo sorgente
     * @param node2 Nodo destinazione
     * @return void* Etichetta dell'arco, NULL se arco non presente o grafo non etichettato
     * 
     * @note Complessità: O(1) assumendo hash table efficiente e grafo sparso
     */
    void* graph_get_label(const Graph gr, const void* node1, const void* node2);

    /**
     * @brief Recupera i nodi adiacenti a un dato nodo
     * 
     * @param gr Grafo
     * @param node Nodo di cui trovare i vicini
     * @return void** Array di puntatori ai nodi adiacenti (deve essere liberato dal chiamante), NULL in caso di errore
     * 
     * @note Complessità: O(1) assumendo hash table efficiente e grafo sparso con liste di adiacenza corte
     */
    void** graph_get_neighbours(const Graph gr, const void* node);

    /**
     * @brief Restituisce il numero di nodi adiacenti a un dato nodo
     * 
     * @param gr Grafo
     * @param node Nodo di cui contare i vicini
     * @return int Numero di nodi adiacenti, -1 in caso di errore
     * 
     * @note Complessità: O(1)
     */
    int graph_num_neighbours(const Graph gr, const void* node);

    /**
     * @brief Libera la memoria occupata dal grafo
     * 
     * @param gr Grafo da liberare
     * 
     * @note Non libera i dati puntati dai nodi ed etichette, solo la struttura del grafo
     */
    void graph_free(Graph gr);

#endif