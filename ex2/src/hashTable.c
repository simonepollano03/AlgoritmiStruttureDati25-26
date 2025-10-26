#include <stdlib.h>
#include <string.h>
    #include "../include/hashTable.h"

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

// Struttura Nodo della lista concatenata 
typedef struct hash_node {
    void* key;
    void* value;
    struct hash_node* next;
} HashNode;

// Struttura tavola hash
struct hash_table {
    HashNode** buckets;      // array liste concatenate
    size_t capacity;
    size_t size;
    int (*compare)(const void *, const void *);
    unsigned long (*hash)(const void *);
};

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
    while (table->compare(current->key, key) == 0) {
        return current->value;
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
            table->buckets[index] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        table->size--;
        return;
    }
    prev = current;
    current = current->next;        

}

int hash_table_size(const HashTable *table) {
    if (table == NULL) return 1;
    
    return (int)table->size;
}

void** hash_table_keyset(const HashTable *table) {
    // Controllo iniziale
    if (table == NULL || table->size == 0) return NULL;

    void **keys = (void **)malloc(table->size * sizeof(void *));
    if (keys == NULL) return NULL;

    size_t key_index = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        HashNode *current = table->buckets[i];
        while (current != NULL) {
            keys[key_index++] = current->key;
            current = current->next;
        }
    }
    
    return keys;
}

void hash_table_free(HashTable *table) {
    if (table == NULL) return;
    
    for (size_t i = 0; i < table->capacity; i++) {
        HashNode* current = table->buckets[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    
    free(table->buckets);
    free(table);
}
