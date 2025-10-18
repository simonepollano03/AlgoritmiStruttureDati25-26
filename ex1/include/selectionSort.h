#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H
    #include <stdlib.h>
    #include <string.h>
    /**
     * @brief Algoritmo Selection Sort - complessità O(n²)
     * 
     * Ordina un array in-place utilizzando l'algoritmo selection sort.
     * Ad ogni iterazione trova l'elemento minimo nella parte non ordinata
     * e lo scambia con l'elemento nella posizione corrente.
     * 
     * @param base Puntatore al primo elemento dell'array
     * @param nitems Numero di elementi nell'array
     * @param size Dimensione in bytes di ogni elemento
     * @param compar Funzione di comparazione che restituisce:
     *               - < 0 se primo elemento < secondo
     *               - = 0 se primo elemento = secondo
     *               - > 0 se primo elemento > secondo
     */
    void selection_sort(void *base, 
                        size_t nitems,
                        size_t size,
                        int (*compar)(const void *, const void *));


#endif