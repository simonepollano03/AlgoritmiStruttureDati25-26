#ifndef CSV_PARSER_H
#define CSV_PARSER_H
    #include "graph.h"
    #include <stdio.h>

    /**
     * @file csv_parser.h
     * @brief Parser per file CSV contenenti informazioni su grafi
     * 
     * Fornisce funzionalità per leggere file CSV nel formato:
     * place1,place2,distance
     * dove ogni riga rappresenta un arco tra due località con una distanza associata.
     */

    /**
     * @brief Struttura che rappresenta un record del file CSV italian_dist_graph.csv
     */
    typedef struct {
        char* place1;       /**< Nome della località sorgente */
        char* place2;       /**< Nome della località destinazione */
        float distance;     /**< Distanza in metri tra le due località */
    } CSVRecord;

    /**
     * @brief Carica un grafo da un file CSV
     * 
     * Legge il file CSV riga per riga e costruisce un grafo non diretto etichettato.
     * Ogni riga del file deve avere il formato: place1,place2,distance
     * 
     * @param filename Nome del file CSV da leggere
     * @param gr Grafo in cui caricare i dati (deve essere già creato)
     * @return int Numero di archi caricati, -1 in caso di errore
     * 
     * @note Il file viene interpretato come grafo NON DIRETTO
     * @note Le distanze vengono memorizzate come etichette degli archi (float*)
     * @note I nomi delle località vengono allocati dinamicamente
     * 
     * @example
     * @code
     * Graph g = graph_create(1, 0, string_compare, string_hash);
     * int edges = load_graph_from_csv("italian_dist_graph.csv", g);
     * printf("Caricati %d archi\n", edges);
     * @endcode
     */
    int load_graph_from_csv(const char* filename, Graph gr);

    /**
     * @brief Legge una singola riga dal file CSV e la parsa
     * 
     * @param file File da cui leggere
     * @param record Struttura in cui salvare i dati letti
     * @return int 1 se lettura riuscita, 0 se fine file, -1 in caso di errore
     * 
     * @note I campi place1 e place2 vengono allocati dinamicamente e devono 
     *       essere liberati dal chiamante
     */
    int read_csv_record(FILE* file, CSVRecord* record);

    /**
     * @brief Libera la memoria allocata per un record CSV
     * 
     * @param record Record da liberare
     */
    void free_csv_record(CSVRecord* record);

    /**
     * @brief Stampa un record CSV (per debug)
     * 
     * @param record Record da stampare
     */
    void print_csv_record(const CSVRecord* record);

#endif