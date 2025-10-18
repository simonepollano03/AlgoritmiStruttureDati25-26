#ifndef QUICK_SORT_H
#define QUICK_SORT_H
    #include <stdlib.h>
    #include <string.h>

    /**
     * @brief Algoritmo QuickSort
     * Ordina un array in-place utilizzando l'algoritmo quicksort.
     * L'implementazione deve essere robusta ed efficiente su tutti i casi di test.
     * La scelta del pivot e della strategia di partizionamento influenza le prestazioni.
     * 
     * @param base Puntatore al primo elemento dell'array
     * @param nitems Numero di elementi nell'array
     * @param size Dimensione in bytes di ogni elemento
     * @param compar Funzione di comparazione che restituisce:
     *               - < 0 se primo elemento < secondo
     *               - = 0 se primo elemento = secondo
     *               - > 0 se primo elemento > secondo
     */
    void quick_sort(void *base,
                    size_t nitems,
                    size_t size,
                    int (*compar)(const void *, const void *));

#endif