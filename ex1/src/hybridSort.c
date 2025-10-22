#include <stdlib.h>
#include <stdio.h>
#include <string.h>
    #include "../include/hybridSort.h"
    #include "../include/selectionSort.h"
    #include "../include/quickSort.h"

static void hybrid_sort_recursive(char *arr, size_t low, size_t high, 
                                  size_t size, size_t k, 
                                  int (*compar)(const void*, const void*)) {
    size_t n = high - low + 1;
    
    if (n < k) {
        selection_sort(arr + low * size, n, size, compar);
        return;
    }
    
    if (low < high) {
        size_t lt, gt;
        
        // USA PARTIZIONE A TRE VIE invece della normale partition
        partition_three_way(arr, low, high, size, compar, &lt, &gt);
        
        if (lt > low) {
            hybrid_sort_recursive(arr, low, lt - 1, size, k, compar);
        }
        if (gt < high) {
            hybrid_sort_recursive(arr, gt + 1, high, size, k, compar);
        }
    }
}

void hybrid_sort(void *base, size_t nitems, size_t size, size_t k, int (*compar)(const void *, const void *)) {
    if (base == NULL || nitems <= 1 || size == 0 || compar == NULL) return;

    // Caso speciale -> uso solo quickSort
    if (k == 0) {
        quick_sort(base, nitems, size, compar);
        return;
    }

    // Caso speciale -> uso solo selectionSort
    if (k >= nitems) {
        selection_sort(base, nitems, size, compar);
        return;
    }

    // Per il resto uso algoritmo ibrido
    char *arr = (char *)base;
    hybrid_sort_recursive(arr, 0, nitems - 1, size, k, compar);
}