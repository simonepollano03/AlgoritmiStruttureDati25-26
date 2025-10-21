#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
    #include "../include/hybridSort.h"
    #include "../include/selectionSort.h"
    #include "../include/quickSort.h"
    #include "../include/recordutils.h"

// Contatori per i test
static int test_passed = 0;
static int test_failed = 0;


// Macro per i test
#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            printf("✓ PASS: %s\n", msg); \
            test_passed++; \
        } else { \
            printf("✗ FAIL: %s\n", msg); \
            test_failed++; \
        } \
    } while(0)
////////

// Funzioni di comparazione per test
int compare_int(const void *a, const void*b) {
    int ia = *(int *)a;
    int ib = *(int *)b;
    
    return (ia > ib) - (ia < ib);
}

float compare_float(const void *a, const void *b) {
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

}

void test_selection_sort_single() {

}

void test_selection_sort_sorted() {

}

void test_selection_sort_reverse() {

}

void test_selection_sort_duplicates() {

}

void test_selection_sort_float() {

}

// ================= TEST QUICKSORT =================
void test_quick_sort_empty() {

}

void test_quick_sort_single() {

}

void test_quick_sort_sorted() {

}

void test_quick_sort_reverse() {

}

void test_quick_sort_duplicates() {

}

void test_quick_sort_random() {

}

void test_quick_sort_large() {

}

// ================= TEST HYBRIDSORT =================
void test_hybrid_sort_k_zero() {

}

void test_hybrid_sort_k_large() {

}

void test_hybrid_sort_k_10() {

}

void test_hybrid_sort_k_50() {

}

void test_hybrid_sort_duplicates() {

}

// ================= TEST RECORDUTILS =================
void test_record_comparators() {

}

void test_get_comparator() {

}

void test_record_sort() {

}


// ================= MAIN TEST =================
int main() {
    
}