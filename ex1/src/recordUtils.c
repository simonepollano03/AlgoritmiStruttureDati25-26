#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/recordUtils.h"

int read_record(FILE *file, Record *record) {
    // Controllo iniziale
    if (file == NULL || record == NULL) return -1;

    // Leggo il file
    size_t read = fread(record, sizeof(Record), 1, file);
    if (read != 1) {
        if (feof(read)) return 0;       // Fine del file
        return -1;                      // Errore di lettura    
    }
    return 1;
}

int write_record(FILE *file, Record *record) {
    // Controllo iniziale
    if (file == NULL || record == NULL) return -1;

    // Scrivo il file
    size_t written = fwrite(record, sizeof(Record), 1, file);
    if (written != 1) 
        return -1;                      // Errore di scrittura
    
    return 1;
}

int load_records_from_file(FILE *file, Record **records, size_t *count) {
    // Controllo iniziale
    if (file == NULL || records == NULL || count == NULL) return -1;

    // Determino la dimensione del file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0 || file_size % sizeof(Record) != 0) {
        fprintf(stderr, "Lunghezza del file invalida");
        return -1;
    }

    *count = file_size /  sizeof(Record);

    // Alloco la memoria per tutti i record
    *records = (Record *)malloc(*count * sizeof(Record));
    if (*records == NULL) {
        fprintf(stderr, "Allocazione della memoria fallita");
        return -1;
    }

    // Leggo tutti i record in una volta
    size_t read = fread(*records, sizeof(Record), *count, file);
    if (read != *count) {
        fprintf(stderr, "Lettura di tutti i records fallita");
        return -1;
    }

    printf("Sono stati caricati %zu records dal file", *count);
    return 0;
}

int save_records_to_file(FILE *file, const Record *records, size_t count) {
    // Controllo iniziale
    if (file == NULL || records == NULL || count == NULL) return -1;

    // Scrivo sul file
    size_t written = fwrite(records, sizeof(Record), count, file);
    if (written != count) {
        fprintf(stderr, "Scrittura di tutti i records fallita");
        return -1;
    }

    printf("Sono stati salvati %zu records nel file", count);
    return 0;
}

void print_record(const Record *record) {
    // Controllo iniziale
    if (record == NULL) return;

    printf("\n\n══════════════════════════════════════════════════════════\n");
    printf("ID: %lu | Field1: %.2f | Field2: %ld | Field3: \"%s\"\n",
            record->id, 
            record->field1,
            record->field2,
            record->field3);
    printf("══════════════════════════════════════════════════════════\n\n");

}


// ==== FUNZIONI DI COMPARAZIONE PER I DIVERSI CAMPI ==== 

int compare_records_by_id(const void *a, const void *b) {
    const Record *r1 = (const Record *)a;
    const Record *r2 = (const Record *)b;

    if (r1->id < r2->id) return -1;
    if (r1->id > r2->id) return  1;
    
    return 0;
}

int compare_records_by_field1(const void *a, const void *b) {
    const Record *r1 = (const Record *)a;
    const Record *r2 = (const Record *)b;

    if (r1->field1 < r2->field1) return -1;
    if (r1->field1 > r2->field1) return  1;

    return 0;
}

int compare_records_by_field2(const void *a, const void *b) {
    const Record *r1 = (const Record *)a;
    const Record *r2 = (const Record *)b;

    if (r1->field2 < r2->field2) return -1;
    if (r1->field2 > r2->field2) return  1;

    return 0;
}

int compare_records_by_field3(const void *a, const void *b) {
    const Record *r1 = (const Record *)a;
    const Record *r2 = (const Record *)b;

    if (r1->field3 < r2->field3) return -1;
    if (r1->field3 > r2->field3) return  1;

    return 0;
}

int (*get_comparator(size_t field))(const void *, const void *) {
    switch (field) {
    case 1:
        return compare_by_id;
    case 2:
        return compare_by_field1;
    case 3:
        return compare_by_field2;
    case 4:
        return compare_by_field3;
    
    default:
        fprintf(stderr, "Numero del campo invalido: %zu\n", field);
        return NULL;
    }
}
