## Relazione Esercizio 3

---

## Struttura della directory

`ex3/`  
├── `src/`               # Codici sorgente (.c) per la priority queue  
├── `include/`           # Files header (.h)  
├── `test/`              # Codice di test con Unity framework e main  
├── `bin/`               # Files eseguibili (generati da make)  
├── `docs/`              # Documentazione generata (HTML)  
├── `Makefile`           # Build automation  
├── `Doxyfile`           # Configurazione Doxygen  
└── `README4.md`         # Questo file

---

## Funzioni principali implementate

### 1. Struttura dati Priority Queue (Heap Ternario)

```c
PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*),
                                     unsigned long (*hash)(const void*));
```

- **Rappresentazione**: array dinamico per l'heap + HashTable per lookup
- **Genericità**: supporta elementi di qualsiasi tipo tramite `void*`
- **Heap Ternario**: ogni nodo ha tre figli

**Struttura interna**
```c
struct priority_queue {
    void** heap;                                // Array dinamico per l'heap
    size_t size;                                // Numero elementi correnti
    size_t capacity;                            // Capacità array
    int (*compare)(const void*, const void*);   // Funzione confronto priorità
    unsigned long (*hash)(const void*);         // Funzione hash
    HashTable* positions;                       // Mappa: elemento -> indice
};
```

### 2. Operazioni sulla Priority Queue
```c
// Inserimento e rimozione
int priority_queue_push(PriorityQueue* pq, void* element);         
void priority_queue_pop(PriorityQueue* pq);                        
void* priority_queue_top(const PriorityQueue* pq);                 

// Ricerca e rimozione specifica
int priority_queue_contains(const PriorityQueue* pq, 
                            const void* element);             
int priority_queue_remove(PriorityQueue* pq, const void* element); 

// Utility
int priority_queue_size(const PriorityQueue* pq);                 
int priority_queue_is_empty(const PriorityQueue* pq);             
void priority_queue_free(PriorityQueue* pq);                      
```
*(\*) Complessità ammortizzata, grazie alla hash table di appoggio*

**Funzioni helper interne**
```c
static void bubble_up(PriorityQueue* pq, size_t index);    // Risale nell'heap
static void heapify_down(PriorityQueue* pq, size_t index); // Scende nell'heap
static void swap_elements(PriorityQueue* pq, size_t i, size_t j);
static void update_position(PriorityQueue* pq, void* element, size_t index);
static int resize_heap(PriorityQueue* pq);                 // Espande capacità
```

### 3. Strutture dati Scheduler
```c
typedef struct task {
    int id;         // Identificatore univoco del task
    int start;      // Tempo di arrivo nel sistema
    int length;     // Durata di esecuzione
    int priority;   // Priorità (più alto = più urgente)
} Task;

typedef struct task_result {
    int id;         // Identificatore del task
    int start_time; // Tempo effettivo di inizio esecuzione
    int end_time;   // Tempo di completamento
} TaskResult;
```
### 4. Operazioni Scheduler
```c
// I/O file CSV
int load_tasks_from_csv(const char* filename, Task** tasks, int* count);
int save_results_to_csv(const char* filename, const TaskResult* results, int count);

// Simulazione scheduling
int run_scheduler(const Task* tasks, int task_count, 
                  TaskResult** results, int* result_count);

// Funzioni di supporto per Priority Queue
int task_compare_priority(const void* a, const void* b);  // Confronto priorità
unsigned long task_hash(const void* element);             // Hash basato su ID
int task_compare_equal(const void* a, const void* b);     // Uguaglianza per ID

// Debug
void print_task(const Task* task);
void print_result(const TaskResult* result);
```

### 5. Dipendenze dall'Esercizio 2
La Priority Queue utilizza la **HashTable** dell'Esercizio 2 per mappare ogni elemento alla sua posizione nell'array heap:
```c
// In priority_queue_create()
pq->positions = hash_table_create(pointer_compare, hash);

// In priority_queue_contains() - O(1) invece di O(n)
return hash_table_contains_key(pq->positions, element);

// In priority_queue_remove() - trova posizione in O(1)
size_t* pos_ptr = (size_t*)hash_table_get(pq->positions, element);
```

Questo permette di evitare la ricerca lineare nell'heap, mantenendo tutte le operazioni con complessità ottimale.

---

## Scelte implementative

### Heap Ternario

A differenza dell'heap binario (2 figli per nodo), l'heap ternario ha **3 figli per nodo**.  
Questo comporta:  

- **Formule per gli indici (0-indexed):**
  - Padre del nodo `i`: `(i - 1) / 3`
  - Primo figlio del nodo `i`: `3 * i + 1`
  - Secondo figlio del nodo `i`: `3 * i + 2`
  - Terzo figlio del nodo `i`: `3 * i + 3`

- **Vantaggi rispetto all'heap binario:**
  - Altezza ridotta: `log3 n` vs `log2 n`, quindi meno livelli da attraversare
  - Potenzialmente migliore utilizzo della cache quando si ha a che fare con array grandi

- **Svantaggi:**
  - Più confronti per livello durante l'heapify-down in quanto si lavora con 3 figli invece che 2
  - Operazioni leggermente più complesse

La **HashTable di appoggio**, derivante dall'esercizio 2, permette di implementare `contains` e `remove` con complessità ottimale, evitando l'utilizzo della ricerca lineare nell'array.

### Complessità Temporali

<div align="center">

| **Operazione** | **Complessità** | **Note**            |
|:--------------:|:---------------:|:-------------------:|
| `push`         | O(log3 n)      | Bubble-up nell'heap |
| `pop`          | O(log3 n)      | Heapify-down nell'heap |
| `top`          | O(1)            | Accesso diretto alla radice |
| `contains`     | O(1)*           | Lookup nella hash table |
| `remove`       | O(log3 n)      | O(1) lookup + O(log3(n))  riorganizzazione |
| `size`         | O(1)            | Valore memorizzato  |
| `is_empty`     | O(1)            | Controllo size == 0 |

*\* Complessità ammortizzata, assumendo hash table efficiente*
</div>  

### Algoritmo dello scheduler

Lo scheduler simula un **esecutore singolo non-preemptive**:

1. Al tempo `t = 0`, l'esecutore è libero
2. Inserisce nella Priority Queue tutti i task con `Start <= t`
3. Se la coda è vuota: avanza il tempo al prossimo arrivo
4. Estrae il task con ***priorità massima***
5. Esegue il task fino al completamento (non-preemptive)
6. Aggiorna `t = t + length` del task eseguito
7. Ripete fino al completamento di tutti i task  

---

## Risultati sperimentali

### Configurazione Test

- **Dataset**: `tasks.csv` con 1.000.000 di task
- **Formato input**: `Id, Start, Length, Priority`

<div align="center">

| **Metrica** | **Valore**  |
|:--------------:|:---------------:|
| Task caricati | 1.000.000 |
| Tempo caricamento | 820.813 ms |
| Tempo scheduling | 1371.308 ms |
| Tempo medio di attesa | 2006.05 |
| Tempo massimo di attesa | 993.958.612 |
| Tempo finale simulazione | 1.000.950.477 |

</div>  

### Analisi dei risultati

1. **Tempo di scheduling**:  
~1.4 secondi per 1 mln di task è un buon risultato, considerando che ogni operazione di push/pop ha complessità O(log3 n) ≈ 13 operazioni per livello.
2. **Tempo massimo di attesa elevato**:  
Questo è dovuto alla natura dello scheduler con priorità. Task con priorità bassa possono attendere molto tempo se continuano ad arrivare task con priorità maggiore.
3. **Efficienza dell'heap ternario**:  
La complessità logaritmica garantisce scalabilità anche con dataset molto grandi.

### Confronto teorico: Heap Binario VS Heap Ternario

Per n = 1.000.000
- Heap Binario: altezza ≈ log2(1.000.000) ≈ 20 livelli
- Heap Ternario: altezza ≈ log3(1.000.000) ≈ 13 livelli  

L'heap ternario ha ~35% di livelli in meno, ma richiede più confronti per livello (3 vs 2).  
In pratica, le performance sono comparabili, con lievi vantaggi per l'heap ternario se si utilizzano grandi dataset dovuto ad una migliore località di cache.

---

## 👥 Autori

[Simone Pollano] – 1068650 – <simone.pollano593@edu.unito.it>
