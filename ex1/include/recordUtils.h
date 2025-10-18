#ifndef RECORD_UTILS_H
#define RECORD_UTILS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>

    /**
     * @brief Struttura Record - esattamente 36 bytes
     * 
     * Rappresenta un record del file binario records.bin
     * Layout in memoria (little-endian):
     * - Byte 0-7:   id (uint64_t)
     * - Byte 8-11:  field1 (float, IEEE 754)
     * - Byte 12-19: field2 (int64_t)
     * - Byte 20-35: field3 (stringa di 16 caratteri, padding con '\0')
     */
    typedef struct {
        uint64_t id;            /**< 8 bytes - intero senza segno a 64 bit */
        float field1;           /**< 4 bytes - numero in virgola mobile */
        int64_t field2;         /**< 8 bytes - intero con segno a 64 bit */
        char field3[16];        /**< 16 bytes - stringa (max 15 caratteri + '\0') */
    } Record;

    /**
     * @brief Legge un singolo record da file binario
     * 
     * @param file Puntatore al file da cui leggere
     * @param record Puntatore alla struttura Record da riempire
     * @return 1 in caso di successo, 0 se EOF, -1 in caso di errore
     */
    int read_record(FILE *file, Record *record);

    /**
     * @brief Scrive un singolo record su file binario
     * 
     * @param file Puntatore al file su cui scrivere
     * @param record Puntatore al record da scrivere
     * @return 1 in caso di successo, -1 in caso di errore
     */
    int write_record(FILE *file, const Record *record);

    /**
     * @brief Carica tutti i record da file in memoria
     * 
     * Alloca dinamicamente la memoria necessaria per contenere tutti i record.
     * La memoria allocata deve essere liberata dal chiamante con free().
     * 
     * @param file Puntatore al file da cui leggere
     * @param records Puntatore all'array di record (allocato dalla funzione)
     * @param count Puntatore alla variabile che conterrà il numero di record letti
     * @return 0 in caso di successo, -1 in caso di errore
     */
    int load_records_from_file(FILE *file, Record **records, size_t *count);

    /**
     * @brief Salva un array di record su file binario
     * 
     * @param file Puntatore al file su cui scrivere
     * @param records Array di record da scrivere
     * @param count Numero di record nell'array
     * @return 0 in caso di successo, -1 in caso di errore
     */
    int save_records_to_file(FILE *file, const Record *records, size_t count);

    /**
     * @brief Stampa un record su stdout (per debugging)
     * 
     * Visualizza tutti i campi del record in formato leggibile.
     * 
     * @param record Puntatore al record da stampare
     */
    void print_record(const Record *record);

    /*========= FUNZIONI PRINCIPALI =========*/
    /**
     * @brief Funzione di comparazione per il campo id
     * 
     * Confronta due record in base al campo id (uint64_t).
     * 
     * @param a Puntatore al primo record
     * @param b Puntatore al secondo record
     * @return < 0 se a.id < b.id, 0 se uguali, > 0 se a.id > b.id
     */
    int compare_by_id(const void *a, const void *b);

    /**
     * @brief Funzione di comparazione per il campo field1
     * 
     * Confronta due record in base al campo field1 (float).
     * 
     * @param a Puntatore al primo record
     * @param b Puntatore al secondo record
     * @return < 0 se a.field1 < b.field1, 0 se uguali, > 0 se a.field1 > b.field1
     */
    int compare_by_field1(const void *a, const void *b);

    /**
     * @brief Funzione di comparazione per il campo field2
     * 
     * Confronta due record in base al campo field2 (int64_t).
     * 
     * @param a Puntatore al primo record
     * @param b Puntatore al secondo record
     * @return < 0 se a.field2 < b.field2, 0 se uguali, > 0 se a.field2 > b.field2
     */
    int compare_by_field2(const void *a, const void *b);

    /**
     * @brief Funzione di comparazione per il campo field3
     * 
     * Confronta due record in base al campo field3 (stringa) in ordine lessicografico.
     * 
     * @param a Puntatore al primo record
     * @param b Puntatore al secondo record
     * @return < 0 se a.field3 < b.field3, 0 se uguali, > 0 se a.field3 > b.field3
     */
    int compare_by_field3(const void *a, const void *b);

#endif