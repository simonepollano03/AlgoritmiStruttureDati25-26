#include <stdlib.h>
#include <stdio.h>
#include <string.h>
    #include "../include/hybridSort.h"
    #include "../include/selectionSort.h"
    #include "../include/quickSort.h"

static void swap(void *a, void *b, size_t size) {
    char *temp = malloc(size);
    if (temp == NULL) return;

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);

    free(temp);
}

static size_t choose_pivot(char *arr, size_t low, size_t high, size_t size, int (*compar)(const void *, const void *)) {
    size_t mid = low + ((high - low)/ 2);       // Previene l'overflow (con low+high/2 overflow)

    void *first  = arr + low * size;
    void *middle = arr + mid * size;
    void *last   = arr + high * size;

    if (compar(first, middle) > 0) {
        swap(first, middle, size);
    }
    if (compar(middle, last) > 0) {
        swap(middle, last, size);
    }
    if (compar(first, middle) > 0) {
        swap(first, middle, size);
    } 

    return mid;

}

static size_t partition(char *arr, size_t low, size_t high, size_t size, int(*compar)(const void *, const void *)) {
    // scelgo il pivot e lo metto al fondo
    size_t pivot_idx = choose_pivot(arr, low, high, size, compar);
    swap(arr + pivot_idx * size, arr + high * size, size);

    void *pivot = arr + high * size;
    size_t i = low;

    // Partizione -> elementi > pivot a destra; <= pivot a sinistra
    for (size_t j = low; j < high; j++) {
        if (compar(arr + j *size, pivot) > 0) {
            if (i != j) {
                swap(arr + i * size, arr + j * size, size);
            }
            i++;
        }
    }
    // Metto il pivot nella posizione corretta
    swap(arr + i * size, arr + high * size, size);

    return i;
}  

static void hybrid_sort_recursive(char *arr, size_t low, size_t high, size_t size, size_t k, int (*compar)(const void *, const void *)) {
    size_t n = high - low + 1;

    if (n < k) {
        selection_sort(arr + low * size, n, size, compar);
        return;
    }

    if (low < high) {
        size_t pivot_idx = partition(arr, low, high, size, compar);
        
        if (pivot_idx > 0) {
            hybrid_sort_recursive(arr, low, pivot_idx - 1, size, k, compar);
        } else {
            hybrid_sort_recursive(arr, pivot_idx - 1, high, size, k, compar);
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
    
    printf("HybridSort completata con successo (k=%zu)\n", k);
}
