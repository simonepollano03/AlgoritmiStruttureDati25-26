#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
    #include "../include/hashTable.h"


#define MAX_WORD_LENGTH 256

static int string_compare(const void *a, const void *b) {
    return strcmp((const void *)a, (const void *)b);
}

static unsigned long string_hash(const void *key) {
    const char *str = (const char *)key;
    unsigned long hash = 5381;
    int c;

    // Scorro ogni carattere della stringa e aggiorno il valore hash
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;        // hash * 33 + c
    }

    return hash;
}

static void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

static void clean_word(char *word) {
    char *src = word;
    char *dst = word;

    while (*src) {
        if (isalpha((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

static char* find_most_frequent_word(HashTable *table, size_t min_length) {
    if (table == NULL) return NULL;

    // Ottengo le keys (parole) presenti nella tabella
    void **keys = hash_table_keyset(table);
    if (keys == NULL) return NULL;

    // Variabili locali 
    int max_frequency = 0;
    char *most_frequent = NULL;
    int size = hash_table_size(table);

    // Scorro tutte le parole prensenti nell'hashTable
    for (int i = 0; i < size; i++) {
        char *word = (char *)keys[i];

        /* Considero solo le parole più lunghe della soglia minima 
           e recupero la frequenza */
        if (strlen(word) >= min_length) {
            int *freq = (int *)hash_table_get(table, word);
            if (freq != NULL && *freq > max_frequency) {
                max_frequency = *freq;
                most_frequent = word;
            }
        }
    }

    free(keys);

    // Ritorno la parola più frequente trovata
    return most_frequent;
}

static int process_file(const char *file_name, HashTable *table) {
    // Apro il file
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR - impossibile aprire il file '%s'\n", file_name);
        return -1;
    }

    // buffer per scorrere le parole
    char word[MAX_WORD_LENGTH];

    // Scorro le parole
    while (fscanf(file, "%255s", word) == 1) {
        clean_word(word);
        to_lowercase(word);

        if (strlen(word) == 0) continue;

        // Cerco se la parola esiste già nell'hashTable
        int *freq = (int *)hash_table_get(table, word);
        
        if (freq != NULL) {
            // Incremento la frequenza
            (*freq)++;
        } else {
            // Alloco memoria per key,value
            char *key = (char *)malloc(strlen(word)+1);
            if (key == NULL) {
                fclose(file);
                return 1;
            }
            strcpy(key, word);

            int *value = (int *)malloc(sizeof(int));
            if (value == NULL) {
                free(key);
                fclose(file);
                return 1;
            }
            *value = 1;

            hash_table_put(table, key, value);
        }
    }

    fclose(file);
    return 0;
}

static void free_table_data(HashTable *table) {
    if (table == NULL) return;

    void **keys = hash_table_keyset(table);
    if (keys == NULL) return;

    int size = hash_table_size(table);

    for (int i = 0; i < size; i++) {
        void *key = keys[i];
        void *value = hash_table_get(table, key);

        free(value);
        free(key);
    }

    free(keys);
}


int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usa: %s <percorso_file> <lunghezza_minima>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_name = argv[1];
    int min_length = atoi(argv[2]);
    if (min_length <= 0) {
        fprintf(stderr, "ERROR - La lunghezza minima deve essere maggiore di 0\n");
        return EXIT_FAILURE;
    }


    // Crea l'hashTable
    HashTable *table = hash_table_create(string_compare, string_hash);
    if (table == NULL) {
        fprintf(stderr, "ERROR - Impossibile creare la tavola hash\n");
        return EXIT_FAILURE;
    }

    printf("════════════════════════════════════════════════════════\n\n");
    // Processa il file
    printf("Lettura del file '%s'...\n", file_name);
    if (process_file(file_name, table) != 0) {
        fprintf(stderr, "ERROR - Errore durante la lettura del file\n");
        return EXIT_FAILURE;
    }

    printf("Parole uniche trovate: %d\n", hash_table_size(table));

    // Trovo la parola più frequente
    char *most_frequent = find_most_frequent_word(table, (size_t)min_length);
    if(most_frequent != NULL) {
        int *freq = (int *)hash_table_get(table, most_frequent);
        printf("\nParola più frequente (lunghezza >= %d): %s\n", min_length, most_frequent);
        printf("Frequenza: %d occorrenze\n", *freq);
    } else {
        printf("\nNessuna parola trovata con lunghezza >= %d", min_length);
    }

    free_table_data(table);
    hash_table_free(table);
    printf("\n════════════════════════════════════════════════════════\n");

    return EXIT_SUCCESS;
}