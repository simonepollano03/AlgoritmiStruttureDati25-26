#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
    #include "../include/hybridSort.h"
    #include "../include/recordUtils.h"

void sort_records(FILE *infile, FILE *outfile, size_t field, size_t k) {
    // Controllo iniziale
    if (infile == NULL || outfile == NULL) {
        fprintf(stderr, "ERROR - sort_records():    File non validi\n");
        return;
    }
    // Carica i record in mememoria da infile
    Record *records = NULL;
    size_t count = 0;

    printf("Caricamento dei records dal file...\n");
    if (load_records_from_file(infile, &records, &count) != 0) {
        fprintf(stderr, "ERROR - sort_records():    load_records_from_file\n");
        return;
    }

    // Ottengo la funzione di comparazione corretta
    int(*compar)(const void *, const void *) = get_comparator(field);
    if (compar == NULL) {
        fprintf(stderr, "ERROR - sort_records():    compar\n");
        free(records);
        return;
    }

    // (Start) Misuro il tempo
    printf("Inizio ordinamento con campo %zu e k=%zu...\n", field, k);
    clock_t start = clock(); 

    // Parte hybridSort
    hybrid_sort(records, count, sizeof(Record), k, compar);

    // (End) Calcola il tempo
    clock_t end = clock();
    double passed_time = ((double)(end-start) / CLOCKS_PER_SEC);
    printf("Ordinamento completato in %.2f secondi\n", passed_time);
    
    // Salvo i record ottenuti in outfile
    printf("Salvataggio dei record ordinati...\n");
    if (save_records_to_file(outfile, records, count) != 0) {
        fprintf(stderr, "ERROR - sort_records():    save_records_to_file\n");
    }

    // Libera la memoria
    free(records);
}


int main(int argc, char *argv[]) {
    // Controllo degli argomenti
    if (argc != 5) {
        fprintf(stderr, "Usa: %s <input_file> <output_file> <field> <k>\n", argv[0]);
        printf("   ├─ <field> : 1=id, 2=field1, 3=field2, 4=field3\n");
        printf("   └─ <k>     : soglia per HybridSort\n");
        printf("            ├─ k=0        : ordina tutto con QuickSort\n");
        printf("            ├─ k>=#record : ordina tutto con SelectionSort\n");
        printf("            └─ altrimenti : usa QuickSort finchè l'array è >=k, poi SelectionSort\n");
        return 1;
    }

    // Parsing degli argomenti
    char *input_file_name  = argv[1];
    char *output_file_name = argv[2];
    size_t field      = (size_t)atoi(argv[3]);
    size_t k          = (size_t)atoi(argv[4]);

    // Validazione del campo
    if (field < 1 || field > 4) {
        fprintf(stderr, "ERROR - field non compreso tra 1 e 4\n");
        return 1;
    }

    // Apro infile e outfile
    FILE *input_file = fopen(input_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "ERROR - impossibile aprire il file di input '%s'\n", input_file_name);
        return 1;
    }

    FILE *output_file = fopen(output_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "ERROR - impossibile aprire il file di output '%s'\n", output_file_name);
        fclose(input_file);
        return 1;
    }

    // Chiamato alla funzione sort_records()
    sort_records(input_file, output_file, field, k);

    // Chiudo i files
    fclose(input_file);
    fclose(output_file);

    printf("\n=== Operazione completata con successo! ===\n");
    printf("  - File di input:  %s\n", input_file_name);
    printf("  - File di output: %s\n", output_file_name);
    printf("  - Campo usato:    %zu\n", field);
    printf("  - Soglia k:       %zu\n", k);
    printf("\nPer visualizzare il file ordinato:\n");
    printf("  - hexdump -C %s | head -n 50\n", output_file_name);
    
    
    return 0;
}