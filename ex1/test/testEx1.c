#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
    #include "../include/hybridSort.h"
    #include "../include/selectionSort.h"
    #include "../include/quickSort.h"
    #include "../include/recordUtils.h"

// Contatori per i test
static int test_passed = 0;
static int test_failed = 0;

// Array per memorizzare i test falliti
#define MAX_FAILED_TESTS 100
static char *failed_tests[MAX_FAILED_TESTS];
static int failed_count = 0;

// Macro per i test
#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("✓ PASS: %s\n", message); \
            test_passed++; \
        } else { \
            printf("✗ FAIL: %s\n", message); \
            test_failed++; \
            if (failed_count < MAX_FAILED_TESTS) { \
                failed_tests[failed_count] = strdup(message); \
                failed_count++; \
            } \
        } \
    } while(0)
////////

// Funzioni di comparazione per test
int compare_int(const void *a, const void*b) {
    int ia = *(int *)a;
    int ib = *(int *)b;
    
    return (ia > ib) - (ia < ib);
}

int compare_float(const void *a, const void *b) {
    float fa = *(float *)a;
    float fb = *(float *)b;
    
    if (fa < fb) return -1;
    if (fa > fb) return  1;

    return 0;
}

int compare_string(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Helper per verificare se un array è ordinato
int is_sorted_int(int *arr, size_t n) {
    for (size_t i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) return 0;
    }
    return 1;
}

int is_sorted_float(float *arr, size_t n) {
    for (size_t i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) return 0;
    }
    return 1;
}

// ================= TEST SELECTIONSORT =================
void test_selection_sort_empty() {
    int nitems = 0;
    selection_sort(NULL, nitems, sizeof(int), compare_int);
    TEST_ASSERT(1, "SelectionSort con array vuoto");
}

void test_selection_sort_single() {
    int arr[]  = {12};
    int nitems = 1;
    selection_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(arr[0] == 12, "SelectionSort con singolo elemento");
}

void test_selection_sort_sorted() {
    int arr[]  = {1, 2, 3, 4, 5};
    int nitems = 5;
    selection_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "SelectionSort con array già ordinato");
}

void test_selection_sort_reverse() {
    int arr[]  = {5, 4, 3, 2, 1};
    int nitems = 5;
    selection_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "SelectionSort con array ordinato inversamente");
}

void test_selection_sort_duplicates() {
    int arr[]  = {3, 1, 4, 1, 5, 9, 2, 5, 6, 3};
    int nitems = 10;
    selection_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "SelectionSort con elementi duplicati");
}

void test_selection_sort_float() {
    float arr[] = {3.14f, 2.71f, 1.41f, 1.73f};
    int nitems  = 4;
    selection_sort(arr, nitems, sizeof(float), compare_float);
    TEST_ASSERT(is_sorted_float(arr, nitems), "SelectionSort con float");
}

void test_selection_sort_large() {
    int nitems = 1000;
    int *arr   = malloc(nitems * sizeof(int));

    for (int i = 0; i < nitems; i++) {
        arr[i] = rand();
    }
    selection_sort(arr, nitems, sizeof(int), compare_int);
    int sorted = is_sorted_int(arr, nitems);
    free(arr);
    TEST_ASSERT(sorted, "SelectionSort con array grande (100 elementi)");
}

// ================= TEST QUICKSORT =================
void test_quick_sort_empty() {
    quick_sort(NULL, 0, sizeof(int), compare_int);
    TEST_ASSERT(1, "QuickSort con array vuoto");
}

void test_quick_sort_single() {
    int arr[]  = {12};
    int nitems = 1; 
    quick_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(arr[0] == 12, "QuickSort con singolo elemento");
}

void test_quick_sort_sorted() {
    int arr[]  = {1, 2, 3, 4, 5};
    int nitems = 5;
    quick_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "QuickSort con array già ordinato");
}

void test_quick_sort_reverse() {
    int arr[]  = {5, 4, 3, 2, 1};
    int nitems = 5;
    quick_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "QuickSort con array ordinato inversamente");
}

void test_quick_sort_duplicates() {
    int arr[]  = {12, 21, 3, 5, 5, 7, 1, 6, 21, 2};
    int nitems = 10;
    quick_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "QuickSort con elementi duplicati");
}

void test_quick_sort_random() {
    int arr[]  = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77};
    int nitems = 14;
    quick_sort(arr, nitems, sizeof(int), compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "QuickSort con array casuale");
}

void test_quick_sort_large() {
    int nitems = 1000;
    int *arr   = malloc(nitems * sizeof(int));

    for (int i = 0; i < nitems; i++) {
        arr[i] = rand();
    }
    quick_sort(arr, nitems, sizeof(int), compare_int);
    int sorted = is_sorted_int(arr, nitems);
    free(arr);
    TEST_ASSERT(sorted, "QuickSort con array grande (100 elementi)");
}

// ================= TEST HYBRIDSORT =================
void test_hybrid_sort_k0() {
    int arr[]  = {5, 2, 8, 1, 9};
    int nitems = 5;
    int k = 0;
    hybrid_sort(arr, nitems, sizeof(int), k, compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "HybridSort con k=0 (-> solo QuickSort)");
}

void test_hybrid_sort_k_high() {
    int arr[]  = {5, 2, 8, 1, 9};
    int nitems = 5;
    int k = 1000;
    hybrid_sort(arr, nitems, sizeof(int), k, compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "HybridSort con k>nitems (-> solo SelectionSort)");
}

void test_hybrid_sort_k10() {
    int arr[]  = {64, 34, 25, 12, 22, 11, 90, 88, 45, 50, 23, 36, 18, 77, 15, 29};
    int nitems = 16;
    int k = 10;
    hybrid_sort(arr, nitems, sizeof(int), k, compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "HybridSort con k=10");
}

void test_hybrid_sort_k50() {
    int nitems = 1000;
    int *arr   = malloc(nitems * sizeof(int));
    int k = 50;

    for (int i = 0; i < nitems; i++) {
        arr[i] = rand();
    }
    hybrid_sort(arr, nitems, sizeof(int), k, compare_int);
    int sorted = is_sorted_int(arr, nitems);
    free(arr);
    TEST_ASSERT(sorted, "hybridSort con array grande (100 elementi) e k=50");
}

void test_hybrid_sort_duplicates_k5() {
    int arr[]  = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};
    int nitems = 15;
    int k = 5;
    hybrid_sort(arr, nitems, sizeof(int), k, compare_int);
    TEST_ASSERT(is_sorted_int(arr, nitems), "HybridSort con duplicati e k=5");
}

// ================= TEST RECORDUTILS =================
void test_record_comparators() {
    Record r1 = {100, 1.5f, -50, "apple"};
    Record r2 = {200, 2.5f, -20, "pear"};

    TEST_ASSERT(compare_records_by_id(&r1, &r2) < 0, "Comparatore id");
    TEST_ASSERT(compare_records_by_field1(&r1, &r2) < 0, "Comparatore field1");
    TEST_ASSERT(compare_records_by_field2(&r1, &r2) < 0, "Comparatore field2");
    TEST_ASSERT(compare_records_by_field3(&r1, &r2) < 0, "Comparatore field3");    
}

void test_get_comparator() {
    TEST_ASSERT(get_comparator(1) == compare_records_by_id, "get_comparator(1)");
    TEST_ASSERT(get_comparator(2) == compare_records_by_field1, "get_comparator(2)");
    TEST_ASSERT(get_comparator(3) == compare_records_by_field2, "get_comparator(3)");
    TEST_ASSERT(get_comparator(4) == compare_records_by_field3, "get_comparator(4)");
    TEST_ASSERT(get_comparator(5) == NULL, "get_comparator con valore invalido");
}

void test_record_sort() {
    int nitems = 3;
    int k = 0;
    
    // Ordina per id
    Record records1[] = {
        {300, 3.0f, 30, "zebra"},
        {100, 1.0f, 10, "hippo"},
        {200, 2.0f, 20, "giraffe"}
    };
    hybrid_sort(records1, nitems, sizeof(Record), k, compare_records_by_id);
    TEST_ASSERT(records1[0].id == 100 && 
                records1[1].id == 200 && 
                records1[2].id == 300, 
                "Ordinamento record per id");

    // Ordina per field1
    Record records2[] = {
        {300, 3.5f, 30, "zebra"},
        {100, 1.2f, 10, "hippo"},
        {200, 2.7f, 20, "giraffe"}
    };
    hybrid_sort(records2, nitems, sizeof(Record), k, compare_records_by_id);
    TEST_ASSERT(records2[0].field1 == 1.2f && 
                records2[1].field1 == 2.7f && 
                records2[2].field1 == 3.5f, 
                "Ordinamento record per field1");

    // Ordina per field2
    Record records3[] = {
        {300, 3.0f, 50, "zebra"},
        {100, 1.0f, -20, "hippo"},
        {200, 2.0f, 15, "giraffe"}
    };
    hybrid_sort(records3, nitems, sizeof(Record), k, compare_records_by_id);
    TEST_ASSERT(records3[0].field2 == -20 && 
                records3[1].field2 == 15  && 
                records3[2].field2 == 50, 
                "Ordinamento record per field2");

    // Ordina per field3
    Record records4[] = {
        {300, 3.0f, 50, "zebra"},
        {100, 1.0f, -20, "hippo"},
        {200, 2.0f, 15, "giraffe"}
    };
    
    /** DEBUG: stampa PRIMA dell'ordinamento *//*
    printf("DEBUG PRIMA: [0]='%s', [1]='%s', [2]='%s'\n", 
           records4[0].field3, records4[1].field3, records4[2].field3);
    *//** DEBUG: END*/
    hybrid_sort(records4, 3, sizeof(Record), 0, compare_records_by_field3);
    
    /** DEBUG: stampa DOPO l'ordinamento *//*
    printf("DEBUG DOPO: [0]='%s', [1]='%s', [2]='%s'\n", 
           records4[0].field3, records4[1].field3, records4[2].field3);
    *//** DEBUG: END */

    TEST_ASSERT(strcmp(records4[0].field3, "giraffe") == 0 &&
                strcmp(records4[1].field3, "hippo") == 0   &&
                strcmp(records4[2].field3, "zebra") == 0, 
                "Ordinamento record per field3");
}


// ================= MAIN TEST =================
int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║                   TEST - ESERCIZIO 1                   ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    // SelectionSort
    printf("--- TEST SELECTIONSORT ---\n");
    test_selection_sort_empty();
    test_selection_sort_single();
    test_selection_sort_sorted();
    test_selection_sort_reverse();
    test_selection_sort_duplicates();
    test_selection_sort_float();
    test_selection_sort_large();
    printf("\n");

    // QuickSort
    printf("--- TEST QUICKSORT ---\n");
    test_quick_sort_empty();
    test_quick_sort_single();
    test_quick_sort_sorted();
    test_quick_sort_reverse();
    test_quick_sort_duplicates();
    test_quick_sort_random();
    test_quick_sort_large();
    printf("\n");

    // HybridSort
    printf("--- TEST HYBRIDSORT ---\n");
    test_hybrid_sort_k0();
    test_hybrid_sort_k_high();
    test_hybrid_sort_k10();
    test_hybrid_sort_k50();
    test_hybrid_sort_duplicates_k5();
    printf("\n");

    // RecordUtils
    printf("--- TEST RECORDUTILS ---\n");
    test_record_comparators();
    test_get_comparator();
    test_record_sort();
    printf("\n");

    // Riepilogo
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  RISULTATI                                             ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║  Test passati:  %-3d                                    ║\n", test_passed);
    printf("║  Test falliti:  %-3d                                    ║\n", test_failed);
    printf("║  Totale:        %-3d                                    ║\n", test_passed + test_failed);
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\n");

    if (test_failed == 0) {
        printf("Tutti i test sono passati!\n\n");
    } else {
        printf("Alcuni test sono falliti...\n\n");
        printf("╔═════════════════════════════════════════════════════════╗\n");
        printf("║  ELENCO TEST FALLITI:                                   ║\n");
        printf("╠═════════════════════════════════════════════════════════╣\n");
        for (int i = 0; i < failed_count; i++) {
            printf("║  %2d. %-50s ║\n", i + 1, failed_tests[i]);
        }
        printf("╚═════════════════════════════════════════════════════════╝\n");
        printf("\n");

        // Libera la memoria dei messaggi
        for (int i = 0; i < failed_count; i++) {
            free(failed_tests[i]);
        }
    }
    return (test_failed == 0) ? 0 : 1;
}