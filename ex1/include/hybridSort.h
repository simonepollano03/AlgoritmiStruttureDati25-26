#ifndef HYBRID_SORT_H
#define HYBRID_SORT_H
    #include <stdlib.h>
    #include <string.h>

/**
 * @brief Algoritmo di HybridSort (QuickSort + SelectionSort)
 * 
 * Combina QuickSort e SelectionSort in base alla dimensione del sottoarray: 
 * per sottoarray con dimensione >= k utilizza QuickSort mentre invece 
 * per sottoarray con dimensione < k utilizza SelectionSort
 * 
 * L'algoritmo passa automaticamente da un metodo all'altro durante la ricorsione.
 * 
 * Casi speciali: se k = 0 usa solo QuickSort su tutto l'array
 * mentre se k > nitems usa solo SelectionSort su tutto l'array
 * 
 * @param base Puntatore al primo elemento dell'array
 * @param nitems Numero di elementi nell'array
 * @param size Dimensione in bytes di ogni elemento
 * @param k Soglia per la selezione dell'algoritmo
 * @param compar Funzione di comparazione che restituisce: 
 *          - < 0 se primo elemento < secondo;
 *          - = 0 se primo elemento = secondo;
 *          - > 0 se primo elemento > secondo.
 */
void hybridsort(void *base,
               size_t nitems,
               size_t size,
               size_t k,
               int (*compar)(const void *, const void *));

#endif