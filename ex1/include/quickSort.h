#ifndef QUICK_SORT_H
#define QUICK_SORT_H
    #include <stdlib.h>
    #include <string.h>

    /**
     * @brief Scambia due elementi in memoria
     * 
     * @param a Puntatore al primo elemento
     * @param b Puntatore al secondo elemento
     * @param size Dimensione in bytes degli elementi
     */
    void swap(void *a, void *b, size_t size);
    
    /**
     * @brief Partizione a tre vie (Dutch National Flag)
     * 
     * Divide l'array in: minori, uguali, maggiori del pivot.
     * 
     * @param base Puntatore all'array
     * @param low Indice iniziale
     * @param high Indice finale
     * @param size Dimensione elemento
     * @param compar Funzione di comparazione
     * @param lt Output: indice fine sezione "minori"
     * @param gt Output: indice inizio sezione "maggiori"
     */
    void partition_three_way(char *base, size_t low, size_t high, size_t size,
                            int (*compar)(const void*, const void*),
                            size_t *lt, size_t *gt);

    /**
     * @brief Algoritmo QuickSort
     * 
     * @param base Puntatore al primo elemento dell'array
     * @param nitems Numero di elementi nell'array
     * @param size Dimensione in bytes di ogni elemento
     * @param compar Funzione di comparazione che restituisce:
     * < 0 se primo elemento < secondo; = 0 se primo elemento = secondo;
     * > 0 se primo elemento > secondo
     */
    void quick_sort(void *base,
                    size_t nitems,
                    size_t size,
                    int (*compar)(const void *, const void *));
    
#endif