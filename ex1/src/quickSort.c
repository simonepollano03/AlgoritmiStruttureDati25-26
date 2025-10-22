#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/quickSort.h"

void swap(void *a, void *b, size_t size) {
    char *temp = malloc(size);
    if (temp == NULL) return;
    
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    
    free(temp);
}

// PARTIZIONE A TRE VIE (Dutch National Flag)
void partition_three_way(char *base, size_t low, size_t high, size_t size, 
                         int (*compar)(const void*, const void*), 
                         size_t *lt, size_t *gt) {
    // Pivot RANDOMIZZATO per evitare degenerazione
    size_t pivot_index = low + rand() % (high - low + 1);
    
    void *pivot = malloc(size);
    if (!pivot) return;
    
    memcpy(pivot, base + pivot_index * size, size);
    
    *lt = low;
    *gt = high;
    size_t i = low;
    
    // Partizione: < pivot | = pivot | > pivot
    while (i <= *gt) {
        int cmp = compar(base + i * size, pivot);
        
        if (cmp < 0) {
            // Elemento minore del pivot -> va a sinistra
            swap(base + i * size, base + (*lt) * size, size);
            (*lt)++;
            i++;
        } else if (cmp > 0) {
            // Elemento maggiore del pivot -> va a destra
            swap(base + i * size, base + (*gt) * size, size);
            (*gt)--;
            // NON incrementare i perché l'elemento scambiato non è stato ancora esaminato
        } else {
            // Elemento uguale al pivot -> resta al centro
            i++;
        }
    }
    
    free(pivot);
}

void quick_sort_recursive(char *arr, size_t low, size_t high, size_t size, 
                         int (*compar)(const void*, const void*)) {
    if (low >= high) return;
    
    size_t lt, gt;
    partition_three_way(arr, low, high, size, compar, &lt, &gt);
    
    // Ordina solo le parti con elementi diversi dal pivot
    if (lt > low) {
        quick_sort_recursive(arr, low, lt - 1, size, compar);
    }
    if (gt < high) {
        quick_sort_recursive(arr, gt + 1, high, size, compar);
    }
}

void quick_sort(void *base, size_t nitems, size_t size, 
               int (*compar)(const void*, const void*)) {
    if (base == NULL || nitems <= 1 || size == 0 || compar == NULL) return;
    
    // Inizializza il seed per la randomizzazione (fallo UNA SOLA VOLTA)
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    
    char *arr = (char *)base;
    quick_sort_recursive(arr, 0, nitems - 1, size, compar);
    
    printf("QuickSort completata con successo!\n");
}