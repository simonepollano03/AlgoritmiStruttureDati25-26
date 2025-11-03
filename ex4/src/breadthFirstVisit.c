#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/csvParser.h"


#define MAX_LINE_LENGTH 1024
#define MAX_PLACE_LENGTH 256


int read_csv_record(FILE* file, CSVRecord* record) {
    // Controllo iniziale
    if (file == NULL || record == NULL) return -1;

    // Buffer lettura righe
    char line[MAX_LINE_LENGTH];

    // Leggo righe
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        return 0;
    }

    // Rimuovo newline se presente
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }
    if (len > 1 && line[len-2] == '\r') {
        line[len-2] = '\0';
    }

    // Parsing del CSV
    char place1[MAX_PLACE_LENGTH];
    char place2[MAX_PLACE_LENGTH];
    float distance;

    // Trovo la prima virgola
    char* first_comma = strchr(line, ',');
    if (first_comma == NULL) return -1;

    // Estraggo place1
    size_t place1_len = first_comma - line;
    if (place1_len >= MAX_PLACE_LENGTH) return -1;
    strncpy(place1, line, place1_len);
    place1[place1_len] = '\0';

    // Trovo la seconda virgola
    char* second_comma = strchr(first_comma + 1, ',');
    if (second_comma == NULL) return -1;
    
    // Estraggo place2
    size_t place2_len = second_comma - (first_comma + 1);
    if (place2_len >= MAX_PLACE_LENGTH) return -1;
    strncpy(place2, first_comma + 1, place2_len);
    place2[place2_len] = '\0';

    // Estraggo distance
    distance = (float)atof(second_comma + 1);

    // Alloco la memoria per i campi
    record->place1 = (char*)malloc(strlen(place1) + 1);
    if (record->place1 == NULL) return -1;
    strcpy(record->place1, place1);
    
    record->place2 = (char*)malloc(strlen(place2) + 1);
    if (record->place2 == NULL)  {
        free(record->place1);
        return -1;
    }
    strcpy(record->place2, place2);

    record->distance = distance;

    return 1;
}

void free_csv_record(CSVRecord* record) {
    if (record == NULL) return;

    if (record->place1 != NULL) {
        free(record->place1);
        record->place1 = NULL;
    }

    if (record->place2 != NULL) {
        free(record->place2);
        record->place2 = NULL;
    }
}

void print_csv_record(const CSVRecord* record) {
    if (record == NULL) return;

    printf("%s , %s , %.2f\n",
            record->place1 ? record->place1 : "NULL",
            record->place2 ? record->place2 : "NULL",
            record->distance);
}

int load_graph_from_csv(const char* file_name, Graph graph) {
    if (file_name == NULL || graph == NULL) return -1;

    FILE* file = fopen(file_name, 'r');
    if (file == NULL) {
        fprintf(stderr, "ERROR - Impossibile aprire il file '%s'\n", file_name);
        return -1;
    }

    int edges_count = 0;
    CSVRecord record;

    while (1) {
        int result = read_csv_record(file, &record);
        
        if (result == 0) break;         // Fine file
        if (result == -1) {             // Errore
            fprintf(stderr, "ERROR - Errore durante la lettura del file CSV\n");
            return -1;
        }

        // Aggiungo i nodi se non esistono già
        graph_add_node(graph, record.place1);
        graph_add_node(graph, record.place2);

        // Alloco la memoeria per la distanza
        float* distance = (float*)malloc(sizeof(float));
        if (distance == NULL) {
            free_csv_record(&record);
            fclose(file);
            return -1;
        }
        *distance = record.distance;

        // Aggiungo l'arco (il grafo non è diretto quindi gestisce automaticamente)
        int edges_result = graph_add_edge(graph, record.place1, record.place2, distance);
        
        if (edges_result == 1) edges_count++;

        // se l'aggiunta fallisce liberlo place1/2
        if (edges_result == -1) {
            free(distance);
            free_csv_record(&record);
        }
    }

    fclose(file);
    return edges_count;
}