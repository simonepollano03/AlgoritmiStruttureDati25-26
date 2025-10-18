#ifndef HYBRID_SORT_H
#define HYBRID_SORT_H
    #include <stdlib.h>
    #include <string.h>

    /**
    * @brief Algoritmo Hybrid Sort (QuickSort + SelectionSort)
    * 
    * Utilizza QuickSort per le sotto-sequenze di dimensione >= k  
    * Utilizza SelectionSort per le sotto-sequenze di dimensione < k
    * 
    * Casi speciali:
    * - k = 0: viene utilizzato esclusivamente QuickSort  
    * - k > nitems: viene utilizzato esclusivamente SelectionSort
    * 
    * @param base Puntatore al primo elemento dell’array
    * @param nitems Numero totale di elementi nell’array
    * @param size Dimensione, in byte, di ciascun elemento
    * @param k Valore soglia per la scelta dell’algoritmo
    * @param compar Puntatore alla funzione di confronto tra due elementi
    */
    void hybrid_sort (void *base, 
                      size_t nitems, 
                      size_t size, 
                      size_t k,
                      int (*compar)(const void *, const void *));
    
#endif