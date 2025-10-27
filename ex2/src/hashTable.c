#include <stdlib.h>
#include <string.h>
    #include "../include/hashTable.h"

/** @brief Capacità iniziale della tavola hash */
#define INITIAL_CAPACITY 16

/** @brief Fattore di carico massimo prima del ridimensionamento (75%) */
#define LOAD_FACTOR 0.75

/**
 * @brief Nodo della lista concatenata per gestire le collisioni
 * 
 * Ogni bucket della tavola hash contiene una lista concatenata di nodi.
 * Questo permette di gestire multiple chiavi che collidono sullo stesso indice.
 */
typedef struct hash_node {
    void* key;                  /**< Puntatore alla chiave */
    void* value;                /**< Puntatore al valore associato */
    struct hash_node* next;     /**< Puntatore al prossimo nodo nella lista */
} HashNode;

/**
 * @brief Struttura interna della tavola hash
 * 
 * Contiene l'array di bucket (liste concatenate), la capacità totale,
 * il numero di elementi e i puntatori alle funzioni di confronto e hash.
 */
struct hash_table {
    HashNode** buckets;                             /**< Array di puntatori alle liste */
    size_t capacity;                                /**< Capacità totale (numero di bucket) */
    size_t size;                                    /**< Numero di elementi inseriti */
    int (*compare)(const void*, const void*);       /**< Funzione di confronto tra chiavi */
    unsigned long (*hash)(const void*);             /**< Funzione di hash */
};

/**
 * @brief Crea un nuovo nodo della lista
 * 
 * @param key Puntatore alla chiave
 * @param value Puntatore al valore
 * @return HashNode* Puntatore al nodo creato, NULL in caso di errore
 */
static HashNode* create_node(const void *key, const void *value) {
    // Alloco memoria dinamica per l'HashNode
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (node == NULL) return NULL;

    // inizializzo le varibili dell'HashNode
    node->key = (void *)key;
    node->value = (void *)value;
    node->next = NULL;

    return node;
}

/**
 * @brief Ridimensiona la tavola hash raddoppiando la capacità
 * 
 * Questa funzione viene chiamata automaticamente quando il fattore di carico
 * supera la soglia definita. Crea una nuova tabella con capacità doppia e
 * reinserisce tutti gli elementi.
 * 
 * @param table Puntatore alla tavola hash da ridimensionare
 * 
 * @note In caso di fallimento dell'allocazione, la tabella originale resta invariata
 */
static void resize(HashTable *table) {
    // Salvo la capacità e i bucket attuale
    size_t old_capacity = table->capacity;
    HashNode **old_buckets = table->buckets;

    // Raddoppio la capacità
    table->capacity *= 2;

    // Alloco un nuovo array di bucket
    table->buckets = (HashNode**)calloc(table->capacity, sizeof(HashNode*));
    // Se fallisco rispristino
    if (table->buckets == NULL) {
        table->buckets = old_buckets;
        table->capacity = old_capacity;
        return;
    }

    table->size = 0;

    // Scorro tutti i vecchi bucket e rinserisco key,value nella nuova tabella
    for (size_t i = 0; i < old_capacity; i++) {
        HashNode *current = old_buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            hash_table_put(table, current->key, current->value);
            free(current);
            current = next;
        }
    }

    free(old_buckets);
}

HashTable* hash_table_create(int (*compare)(const void *,const void *), unsigned long (*hash)(const void*)) {
    // Controllo iniziale
    if (compare == NULL || hash == NULL) return NULL;

    // Alloco memoria dinamica per HashTable
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) return NULL;

    // Inizializzo le variabili dell'HashTable
    table->capacity = INITIAL_CAPACITY;
    table->size = 0;
    table->compare = compare;
    table->hash = hash;

    table->buckets = (HashNode**)calloc(table->capacity, sizeof(HashNode*));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

void hash_table_put(HashTable *table, const void *key, const void *value) {
    // Controllo iniziale
    if (table == NULL || key == NULL) return;

    // Controllo ridimensionamento
    if ((float)table->size / table->capacity > LOAD_FACTOR) {
        resize(table);
    }
    // Calcolo valore hash della key
    unsigned long hash_value = table->hash(key);
    size_t index = hash_value % table->capacity;

    // Cerco se la chiave esiste
    HashNode* current = table->buckets[index];
    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {
            current->value = (void *)value;
            return;
        }
        current = current->next;
    }

    // Inserisci il nuovo nodo in testa
    HashNode* new_node = create_node(key, value);
    if (new_node == NULL) return;

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;
}


void* hash_table_get(const HashTable *table, const void *key) {
    // Controllo iniziale
    if (table == NULL || key == NULL) return NULL;

    // Calcola valore hash della key
    unsigned long hash_value = table->hash(key);
    size_t index = hash_value % table->capacity;

    // Cerco la chiave e ritorno il valore corrente
    HashNode *current = table->buckets[index];
    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL;
}

int hash_table_contains_key(const HashTable *table, const void *key) {
    return hash_table_get(table, key) != NULL;
}

void hash_table_remove(HashTable *table, const void *key) {
    // Controllo iniziale
    if (table == NULL || key == NULL) return;

    // Calcola valore hash della key
    unsigned long hash_value = table->hash(key);
    size_t index = hash_value % table->capacity;

    HashNode *current = table->buckets[index];
    HashNode *prev = NULL;

    // Cerco la chiave ed elimino il valore
    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {
            // Trovo la chiave da rimuovere
            if (prev == NULL) {
                // Primo nodo della lista
                table->buckets[index] = current->next;
            } else {
                // Nodo intermedio o finale
                prev->next = current->next;
            }
            free(current);
            table->size--;
            return;
        }
        prev = current;
        current = current->next;        
    }
}

int hash_table_size(const HashTable *table) {
    if (table == NULL) return 0;
    
    return (int)table->size;
}

void** hash_table_keyset(const HashTable *table) {
    // Controllo iniziale
    if (table == NULL || table->size == 0) return NULL;

    // Alloca la memoria per le chiavi
    void **keys = (void **)malloc(table->size * sizeof(void *));
    if (keys == NULL) return NULL;

    size_t key_index = 0;

    // Scorri tutti i bucket dell'hashTable
    for (size_t i = 0; i < table->capacity; i++) {
        HashNode *current = table->buckets[i];
        
        // Scorre la lista collegata la bucket corrente
        while (current != NULL) {
            
            // Aggiungi la chiave corrente all'array di chiavi
            keys[key_index++] = current->key;
            current = current->next;
        }
    }
    
    return keys;
}

void hash_table_free(HashTable *table) {
    // Controllo iniziale
    if (table == NULL) return;
    
    // Scorri i bucket dell'hashTable
    for (size_t i = 0; i < table->capacity; i++) {
        HashNode* current = table->buckets[i];
        
        // Libera tutti i nodi della lista collegata al bucket corrente
        while (current != NULL) {
            HashNode* next = current->next;      // Salva il puntatore al nodo successivo
            free(current);                       // Libera il nodo corrente
            current = next;                      // Passa al nodo successivo
        }
    }
    
    free(table->buckets);
    free(table);
}
