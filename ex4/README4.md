## Relazione Esercizio 4

---

## Struttura della directory

`ex4/`  
├── `src/`               # Codici sorgente (.c) per il grafo  
├── `include/`           # Files header (.h)  
├── `test/`              # Codice di test con Unity framework e main  
├── `bin/`               # Files eseguibili (generati da make)  
├── `docs/`              # Documentazione generata (HTML)  
├── `Makefile`           # Build automation  
├── `Doxyfile`           # Configurazione Doxygen    
└── `README4.md`         # Questo file  

---

## Implementazione

### Funzioni principali implementate

#### 1. Struttura dati del Grafo (grafo sparso)
```c
Graph graph_create(int labelled, int directed,
                  int (*compare)(const void*, const void*),
                  unsigned long (*hash)(const void*));
```

- **Rappresentazione**: liste di adiacenza implementate con hashTable (esercizio 2)
- **Genericità**: supporta nodi ed etichette di qualsiasi tuìipo tramite `void*`
- **Flessibilità**: si possono avere grafi diretti/non diretti, etichettati/non etichettati
- **Ottimizzazione per grafi sparsi**: complessità *O(1)* per accesso ai vicini

**Struttura interna**
```c
struct graph {
    int labelled;                      // Flag: grafo etichettato
    int directed;                      // Flag: grafo diretto
    HashTable* adjacency_map;          // Mappa: nodo -> lista di adiacenza
    int num_nodes;                     // Contatore nodi
    int num_edges;                     // Contatore archi
    int (*compare)(const void*, const void*);
    unsigned long (*hash)(const void*);
};
```
**Lista di adiacenza**
```c
typedef struct adjacency_node {
    void* dest;                   // Nodo destinazione
    void* label;                  // Etichetta arco
    struct adjacency_node* next;  // Prossimo nodo
} AdjacencyNode;
```

#### 2. Operazioni sul grafo
```c
// Gestione nodi
int graph_add_node(Graph gr, const void* node);                    // O(1)
int graph_contains_node(const Graph gr, const void* node);         // O(1)
int graph_remove_node(Graph gr, const void* node);                 // O(N)
void** graph_get_nodes(const Graph gr);                            // O(N)

// Gestione archi
int graph_add_edge(Graph gr, const void* n1, const void* n2, 
                   const void* label);                             // O(1)*
int graph_contains_edge(const Graph gr, const void* n1, 
                        const void* n2);                           // O(1)*
int graph_remove_edge(Graph gr, const void* n1, const void* n2);   // O(1)*
void* graph_get_label(const Graph gr, const void* n1, 
                      const void* n2);                             // O(1)*

// Adiacenza
void** graph_get_neighbours(const Graph gr, const void* node);     // O(1)*
int graph_num_neighbours(const Graph gr, const void* node);        // O(1)
```
*(* * *)* *Complessità assumendo hash table efficiente e grafo sparso*

**Gestione grafi non diretti**
- Quando si aggiunge un arco (A,B) con `graph_add_edge()`, viene automaticamente aggiunto anche (B,A)
- La rimozione di un arco rimuove entrambe le direzioni
- In `graph_get_edges()`, viene restiuito solo un arco per coppia, andando ad evitare i duplicati

#### 3. Breadth-First Search
```c
void** breadth_first_visit(Graph gr, void* start,
                          int (*compare)(const void*, const void*),
                          unsigned long (*hash)(const void*));
```

#### Implementazione
- **Strutture ausiliarie**:
    - Coda FIFO, implementata con lista concatenata
    - Hash Table per tracciare i nodi visitati

- **Algoritmo**: 
    1. Marca il nodo iniziale come visitato e lo inserisce nella coda;
    2. Finche la coda non è vuota:
        - Estrae un nodo della coda
        - Lo aggiunge al risultato
        - Per ogni vicino non visitato: lo marca e lo inserisce in coda
    3. Restituisce l'array di nodi ordinati per livello di visita  
**Complessità**: O(V + E) dove V rappresenta il numero di nodi ed E rappresenta il numero di archi.

#### 4. Parser CSV
```c
int load_graph_from_csv(const char* filename, Graph gr);
int read_csv_record(FILE* file, CSVRecord* record);
```

#### Processo di caricamento:
1. Parsing manuale delle righe
2. Allocazione dinamica per nomi delle città
3. Aggiunta automatica dei nodi al grafo, qualora non esistessero
4. Creazione degli archi non diretti con distanze come etichette

---

## Scelte Implementative
Sono state implementate le liste di adiacenza con l'utilizzo dell'hash table siccome rispetto ad altre techiche utilizza uno spazio ridotto ( O(V + E) ) quindi è ideale per grafi sparsi.

Grazie all'utilizzo dell'hash table, l'accesso alla lista dei vicini di un nodo è in tempo O(1) e l'iterazione sui nodi adiacenti costa solo O(grado) quindi proporzionale ai soli archi realmente presenti.

L'unico svantaggio incontrato è la presenza di un piccolo overhead introdotto dall'hash table che però è compensato dai vantaggi citati.

Questa scelta è particolarmente adatta al dataset `italian_dist_graph.csv`, che rappresenta un grafo estremamente sparso con circa 18.000 nodi e un numero medio di vicini < 10.  

---

## Analisi dei risultati
Il programma è stato testato con il file csv `italian_dist_graph.csv` per verificare la correttezza.
<div align="center">

| **Metrica** | **Valore** |
|:--------------------:|:------------------:|
| Nodi totali          | 18.640             |
| Archi totali         | 48.055              |
| Nodi visitati da torino              | 18.636            |
</div>  

Risultato per `start_city` = `torino`

``` text
Primi nodi visitati:
   1. torino
   2. revigliasco
   3. lingotto
   4. l'eremo
   5. ceronda
   6. venaria reale
   7. san mauro torinese
   8. vanchiglia
   9. superga
  10. trofarello
  ...
  (altri 18626 nodi visitati)
```

**Note**:
- La ricerca della città è *case-insensitive*
- Se la città non esiste vengono mostrati suggerimenti con una struttura simile

Dal risultato si evince che la BFV visita le città in ordine di "distanza topologica" da `start_city`:
- **Liv 0**: torino (`start_city`)
- **Liv 1**: revigliasco, lingotto, ...
- **Liv 2**: Città collegata ai vicini di torino
- ...
- **Liv N**: Località più remote (es: lipari, lampedusa, catanzaro (Sicilia / Calabria))

---
## 👥 Autori

[Simone Pollano] – 1068650 – <simone.pollano593@edu.unito.it>
