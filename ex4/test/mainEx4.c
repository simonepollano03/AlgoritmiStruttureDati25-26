#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/graph.h"
    #include "../include/breadthFirstVisit.h"
    #include "../include/csvParser.h"

/**
 * @brief Funzione di confronto per stringhe
 */
static int string_compare(const void* a, const void* b) {
    return strcmp((const void*)a, (const void*)b);
}

/**
 * @brief Funzione di hash delle stringhe (dbj2)
 */
static unsigned long string_hash(const void* key) {
    const char* str = (const char*)key;
    unsigned long hash = 5381;
    int c;

    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * @brief Salva i risultati della BFV in un file
 * 
 * @param output_file Percorso del file di destinazione in cui salvare i risultati.
 * @param visited Array di puntatori ai nodi visitati.
 * @param count Numero di elementi presenti nell'array `visited`.
 * 
 * @return 0 in caso di successo o -1 in caso di fallimento
 */
static int save_bfv_results(const char* output_file, void** visited, int count) {
    FILE* file = fopen(output_file, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR- impossibile creare il file '%s'\n", output_file);
        return -1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", (char*)visited[i]);
    }

    fclose(file);
    return 0;
}

/**
 * @brief Conta il numero di elementi nell'array ritornato da BFV
 */
static int count_visited_nodes(void** visited) {
    if (visited == NULL) return 0;

    int count = 0;
    while (visited[count] != NULL) {
        count++;
    }

    return count;
}

/**
 * @brief Stampa le statistiche del grafo
 */
static void print_graph_stats(Graph gr) {
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║              STATISTICHE DEL GRAFO                   ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");
    
    printf("  Tipo: %s\n", graph_is_directed(gr) ? "Diretto" : "Non diretto");
    printf("  Etichettato: %s\n", graph_is_labelled(gr) ? "Sì" : "No");
    printf("  Numero di nodi: %d\n", graph_num_nodes(gr));
    printf("  Numero di archi: %d\n", graph_num_edges(gr));
    
    printf("\n");
}

/**
 * @brief Normaliiza il nome della città 
 * (rimuove spazi extra, converte in lowercase)
 */
static char* normalize_city_name(const char* city_name) {
    if (city_name == NULL) return NULL;

    char* normalized = (char*)malloc(strlen(city_name) + 1);
    if (normalized == NULL) return NULL;

    int j = 0;
    for (int i = 0; city_name[i] != '/0'; i++) {
        
        // Salta spazi multipli
        if (city_name[i] == ' ' && (i == 0 || city_name[i-1] == ' ')) {
            continue;
        }

        // Converti in lowercase
        if (city_name[i] >= 'A' && city_name[i] <= 'Z') {
            normalized[j++] = city_name[i] + 32;
        } else {
            normalized[j++] = city_name[i];
        }
    }
    
    // Rimuovi spazio finale se presente
    if (j > 0 && normalized[j-1] == ' ') {
        j--;
    }

    normalized[j] = '/0';
    return normalized;
}

/**
 * @brief Cerca un nodo nel grafo ignorando maiuscole e/o minuscole
 */
static void* find_node_case_insensitive(Graph gr, const char* city_name) {
    void** nodes = graph_get_nodes(gr);
    if (nodes == NULL) return NULL;

    int num_nodes = graph_num_nodes(gr);
    char* normalized_search = normalize_city_name(city_name);

    void* found = NULL;

    for (int i = 0; i < num_nodes; i++) {
        char* normalized_node = normalize_city_name((char*)nodes[i]);

        if (normalized_node != NULL && normalized_search != NULL) {
            if (strcmp(normalized_node, normalized_search) == 0) {
                found = nodes[i];
                free(normalized_node);
                break;
            }

            free(normalized_node);
        }
    }

    free(normalized_search);
    free(nodes);

    return found;
}

/**
 * @brief Stampa suggerimenti per città simili
 */
static void suggest_similar_cities(Graph gr, const char* city_name) {
    void** nodes = graph_get_nodes(gr);
    if (nodes == NULL) return NULL;

    int num_nodes = graph_num_nodes(gr);
    printf("\n Città disponibili che iniziano con'%c': \n", city_name[0]);

    int found = 0;
    for (int i = 0; i < num_nodes && found < 10; i++) {
        char* node = (char*)nodes[i];
        if (node[0] == city_name[0] || node[0] == (city_name[0] + 32) || node[0] == (city_name[0] - 32)) {
                printf("    - %s\n", node);
                found++;
        }
    }

    if (found == 0) {
        printf("    (nessuma città trovata)\n");
    }

    free(nodes);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usa: %s <file_csv> <città_partenza> <output_file>\n", argv[0]);
        printf("    Esempio:\n");
        printf("      %s italian_dist_graph.csv torino output.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* csv_file = argv[1];
    const char* start_city = argv[2];
    const char* output_file = argv[3];

    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║           ESERCIZIO 4 - GRAFI E BFV                  ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");

    // Creo il grafo
    printf("--> Creazione del grafo...\n");
    Graph graph = graph_create(1, 0, string_compare, string_hash);
    if (graph == NULL) {
        fprintf(stderr, "ERROR - Impossibile creare il grafo\n");
        return EXIT_FAILURE;
    }

    // Carico i dati dal csv
    printf("--> Caricamento dati da '%s'...\n");
    int edges_loaded = load_graph_from_csv(csv_file, graph);
    if (edges_loaded < 0) {
        fprintf(stderr, "ERROR - Impossibile caricare il file CSV");
        return EXIT_FAILURE;
    }

    printf("Caricati %d archi\n", edges_loaded);
    
    // Stampa statistiche grafo
    print_graph_stats(graph);

}