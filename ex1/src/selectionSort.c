#include <stdlib.h>
#include <stdio.h>
#include <string.h>
    #include "../include/selectionSort.h"

void selection_sort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *)) {
    // Controllo iniziale
    if (base == NULL || nitems <= 1 || size == 0 || compar == NULL) return;

    // Allocazione puntatori alla memoria
    char *arr  = (char *)base;
    char *temp = malloc(size);

    if (temp == NULL) {
        fprintf(stderr, "ERROR - SelectionSort():   Allocazione memoria fallita\n");
        return;
    }

    // Ciclo principale
    for (size_t i = 0; i < nitems-1; i++) {
        size_t min_indx = i;

        // Trovo l'indice dell'elemento minimo nel resto dell'array 
        for (size_t j = i + 1; j < nitems; j++) {
            if (compar(arr + j * size, arr + min_indx * size) < 0) {
                min_indx = j;
            }
        }

        // Scambio l'elemento corrente con il minimo trovato (j)
        if (min_indx != i) {
            memcpy(temp, arr + i * size, size);
            memcpy(arr + i * size, arr + min_indx * size, size);
            memcpy(arr + min_indx * size, temp, size);
        }
    }

    free(temp);
}