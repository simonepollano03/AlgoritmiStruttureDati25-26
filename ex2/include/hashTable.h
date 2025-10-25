#ifndef HASHTABLE_H
#define HASHTABLE_H

    /**
     * @brief Struttura opaca che rappresenta una 
     * tavola hash con concatenamento
     */
    typedef struct hash_table HashTable;

    /**
     * @brief Crea una nuova tavola hash
     * 
     * @param compare Funzione di confronto tra chiavi (ritorna 0 se uguali)
     * @param hash Funzione di hash per le chiavi
     * @return HashTable* Puntatore alla tavola hash creata, NULL in caso di errore
     */
    HashTable* hash_table_create(int (*compare)(const void*,const void*),
                                unsigned long (*hash)(const void*));

    /**
     * @brief Inserisce o aggiorna una coppia chiave-valore nella tavola hash
     * 
     * @param table Tavola hash
     * @param key Chiave da inserire
     * @param value Valore associato alla chiave
     */
    void hash_table_put(HashTable* table, const void* key, const void* value);

    /**
     * @brief Recupera il valore associato a una chiave
     * 
     * @param table Tavola hash
     * @param key Chiave da cercare
     * @return void* Valore associato, NULL se la chiave non esiste
     */
    void* hash_table_get(const HashTable* table, const void* key);

    /**
     * @brief Verifica se una chiave è presente nella tavola hash
     * 
     * @param table Tavola hash
     * @param key Chiave da cercare
     * @return int 1 se presente, 0 altrimenti
     */
    int hash_table_contains_key(const HashTable* table, const void* key);

    /**
     * @brief Rimuove una chiave e il suo valore dalla tavola hash
     * 
     * @param table Tavola hash
     * @param key Chiave da rimuovere
     */
    void hash_table_remove(HashTable* table, const void* key);

    /**
     * @brief Restituisce il numero di coppie chiave-valore nella tavola
     * 
     * @param table Tavola hash
     * @return int Numero di elementi
     */
    int hash_table_size(const HashTable* table);

    /**
     * @brief Restituisce un array di tutte le chiavi presenti
     * 
     * @param table Tavola hash
     * @return void** Array di chiavi (deve essere liberato dal chiamante)
     */
    void** hash_table_keyset(const HashTable* table);

    /**
     * @brief Libera la memoria occupata dalla tavola hash
     * 
     * @param table Tavola hash da liberare
     */
    void hash_table_free(HashTable* table);

#endif