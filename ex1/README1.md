## Relazione Esercizio 1

---

## Struttura della directory

`ex1/`  
├── `src/`               # Codici sorgente (.c) per gli algoritmi di ordinamento  
├── `include/`           # Files header (.h)  
├── `test/`              # Codice di test e main   
├── `bin/`               # Files oggetto (generati da make)  
├── `Makefile`           # Build automation  
└── `README1.md`         # Questo file 

---

## Implementazione

### Funzioni principali implementate

#### 1. QuickSort con partizione a tre vie
```c
void quick_sort(void *base, size_t nitems, size_t size, 
                int (*compar)(const void*, const void*));
```

- **Pivot randomizzato** per evitare casi peggiori
- **Partizione a tre vie**: elementi minori, uguali e maggiori del pivot separati
- **Complessità**: O(n log n) anche con duplicati
- **Ottimizzazione**: i duplicati (isolati al centro) non vengono più processati

#### 2. SelectionSort
```c
void selection_sort(void *base, size_t nitems, size_t size,
                    int (*compar)(const void*, const void*));
```

- **Implementazione classica**: trova il minimo e lo scambia con l'elemento corrente
- **Complessità**: O(n²) sempre
- **Vantaggi**: overhead minimo, efficiente solo su array molto piccoli (< 50 elementi)

#### 3. HybridSort
```c
void hybrid_sort(void *base, size_t nitems, size_t size, size_t k,
                int (*compar)(const void*, const void*));
```

- **Strategia**: usa QuickSort per sottoarray con n ≥ k, SelectionSort per n < k
- **Casi speciali**:
  - k = 0: usa solo QuickSort
  - k >= n: usa solo SelectionSort
- **Ricorsione ibrida**: passa automaticamente da un algoritmo all'altro durante la suddivisione

#### 4. Gestione Record
```c
int load_records_from_file(FILE *file, Record **records, size_t *count);
int save_records_to_file(FILE *file, const Record *records, size_t count);
int (*get_comparator(size_t field))(const void*, const void*);
```

- **Lettura ottimizzata**: un'unica `fread()` per tutti i record (18M record in ~1 secondo)
- **Comparatori dedicati**: funzioni separate per ogni campo (id, field1, field2, field3)
- **Selezione dinamica**: `get_comparator()` restituisce il comparatore corretto in base al campo

---

## Problematiche riscontrate

**Situazione iniziale:** L'ordinamento del dataset (18M record) si bloccava indefinitamente, rendendo impossibile completare l'esecuzione.

**Analisi del problema:** 

La prima implementazione utilizzava la strategia di partizionamento "mediana di tre" con partizione a due vie:
```c
// Implementazione iniziale - PROBLEMATICA
size_t partition(char *arr, size_t low, size_t high, size_t size, 
                int (*compar)(const void *, const void *)) {
    // Scelta pivot: mediana di primo, medio e ultimo elemento
    size_t mid = low + (high - low) / 2;
    
    // Partizione: [elementi <= pivot] [elementi > pivot]
    // ...
}
```

Questa strategia presentava un difetto critico: in presenza di **valori duplicati** (molto frequenti nel campo `id` del dataset), tutti gli elementi uguali al pivot venivano posizionati nella stessa partizione. Questo causava:
- Partizioni fortemente sbilanciate
- Albero di ricorsione degenerato (profondità O(n) invece di O(log n))
- Complessità effettiva O(n²) → oltre 300 trilioni di operazioni

**Soluzione implementata:**

Adottata la **partizione a tre vie** (Dutch National Flag Algorithm):
```c
void partition_three_way(char *base, size_t low, size_t high, size_t size,
                        int (*compar)(const void*, const void*),
                        size_t *lt, size_t *gt) {
    // Pivot randomizzato
    size_t pivot_index = low + rand() % (high - low + 1);
    void *pivot = malloc(size);
    memcpy(pivot, base + pivot_index * size, size);
    
    *lt = low;   // Fine zona "minori"
    *gt = high;  // Inizio zona "maggiori"
    size_t i = low;
    
    // Invariante: [low..lt-1] < pivot, [lt..i-1] = pivot, 
    //             [i..gt] non esaminati, [gt+1..high] > pivot
    while (i <= *gt) {
        int cmp = compar(base + i * size, pivot);
        if (cmp < 0) {
            swap(base + i * size, base + (*lt) * size, size);
            (*lt)++;
            i++;
        } else if (cmp > 0) {
            swap(base + i * size, base + (*gt) * size, size);
            (*gt)--;
        } else {
            i++;  // Elemento uguale: rimane al centro
        }
    }
    free(pivot);
}
```

La partizione a tre vie produce: `[< pivot] [= pivot] [> pivot]`

Gli elementi uguali al pivot vengono **isolati nella sezione centrale** e non vengono più processati nelle chiamate ricorsive:
```c
void quick_sort_recursive(char *arr, size_t low, size_t high, size_t size,
                         int (*compar)(const void*, const void*)) {
    if (low >= high) return;
    
    size_t lt, gt;
    partition_three_way(arr, low, high, size, compar, &lt, &gt);
    
    // Ricorsione SOLO su elementi diversi dal pivot
    if (lt > low) {
        quick_sort_recursive(arr, low, lt - 1, size, compar);  // Minori
    }
    // [lt...gt] già ordinati (tutti uguali al pivot) → saltati!
    if (gt < high) {
        quick_sort_recursive(arr, gt + 1, high, size, compar); // Maggiori
    }
}
```

**Risultato:**
- ❌ **Prima** (partizione a due vie): blocco totale, impossibile completare
- ✅ **Dopo** (partizione a tre vie): 5-7 secondi su 18M record, complessità O(n log n) garantita

--- 

## Analisi Dei risultati

Nei test che sono stati effettuati è stato usato un dataset di **18.000.000 record** (~648 MB).

### Tempi di esecuzione per campo e valore di k:**


<div align="center">

| **k** | **field1 (id)** | **field2 (float)** | **field3 (int64)** | **field4 (string)** |
|:-----:|:---------------:|:-----------------:|:-----------------:|:-------------------:|
| 0 | 7.25s | 4.78s | 7.83s | 6.82s |
| 10 | 7.11s | 5.16s | 7.18s | 5.96s |
| 50 | 7.00s | 4.92s | 7.09s | 6.13s |
| 100 | 7.28s | 5.16s | 7.19s | 6.60s |
| 500 | 11.57s | 7.60s | 10.99s | 12.11s |
| 1.000 | 15.12s | 11.60s | 16.62s | 18.46s |
| 5.000 | 61.32s | 52.09s | 71.09s | 91.09s |
| 10.000 | 127.66s | 108.33s | 144.22s | 197.10s |
| 1.000.000 | TIMEOUT | TIMEOUT | TIMEOUT | TIMEOUT |

</div>

  
### Caso con k=1000000  
Con k=1.000.000, l'algoritmo ibrido (HybridSort) tenta di usare il SelectionSort su sottoarray di dimensione fino a ~1M elementi.

### Analisi della complessità**  
SelectionSort: O(n²):  
- Con (k) n=1.000.000 si effettuano circa 10¹² operazioni, quindi un trilione di operazioni quindi il tempo di esecuzione è **> 30 minuti**, potrebbe anche metterci ore.

### Conclusione: 
Utilizzare k=1.000.000 è impraticabile per dataset di questa dimensione.  
Il SelectionSort è più efficiente in situazioni in cui il valore di k, quindi la dimensione del sottoarray, è inferiore di 10000.

####  Valore ottimale di k
Analizzando i tempi di esecuzione si può osservare che il valore di k ottimale è: 
<div align="center">
    
***50 <= k <= 100***.  
</div>

- Per k ∈ [10, 100]: tempi migliori (~5-7 secondi)
- k=50 risulta essere il valore più stabile su tutti i campi
- Per k < 50: overhead di QuickSort su array piccoli
- Per k > 100: SelectionSort O(n²) inizia a dominare  


#### Differenze tra i campi

- **field2 (float)**: più veloce in assoluto (*k=0, t=4.78 s*)
    - Vengono effettuati confronti numerici più rapidi  
- **field4 (string)**: più lento (*k=0, t=6.82 s*, *k=10.000, t=197 s*)
    - La funzione `strcmp()` è più costosa per confronti nuemrici
    - Degrada più rapidamente all'aumentare di k  
- **field1 (id)** e **field3 (int64)**: hanno performance simili
    - Confronti tra interi a 64 bit

#### Confronto QuickSort puro vs HybridSort

- **k=0 (solo QuickSort)**: 4.78-7.83 secondi
- **k=50 (HybridSort ottimale)**: 4.92-7.09 secondi

**Risultato:** L'approccio ibrido offre vantaggi **minimi** o **nulli** rispetto al QuickSort puro.

L'ipotesi iniziale (SelectionSort più efficiente su array piccoli per minor overhead) si rivela **parzialmente falsa** con i dati reali:

1. Il QuickSort con partizione a tre vie è già molto efficiente
2. Il costo O(n²) di SelectionSort supera rapidamente i benefici del minor overhead
3. Solo per k molto piccoli (≤ 100) si osservano miglioramenti trascurabili

---

## Compilazione ed Esecuzione

### Compilazione

Il progetto utilizza un Makefile per automatizzare la compilazione. I comandi disponibili sono:
```bash
# Compila tutti gli eseguibili (main_ex1 e test_ex1)
make all

# Pulisce i file compilati
make clean

# Compila ed esegue i test automatici
make test
```

**Output della compilazione:**
```bash
$ make all
mkdir -p bin
gcc -Wall -Wextra -Werror -std=c11 -O2 -g -Iinclude \
    -o bin/mainEx1 src/selectionSort.c src/quickSort.c \
    src/hybridSort.c src/recordUtils.c test/mainEx1.c -lm
gcc -Wall -Wextra -Werror -std=c11 -O2 -g -Iinclude \
    -o bin/testEx1 src/selectionSort.c src/quickSort.c \
    src/hybridSort.c src/recordUtils.c test/testEx1.c -lm
✓ Compilazione completata con successo
```

### Esecuzione

#### 1. Test automatici

Per verificare la correttezza dell'implementazione:
```bash
$ make test
# oppure
$ ./bin/testEx1
```

#### 2. Ordinamento dei record

**Sintassi:**
```bash
./bin/mainEx1 <input_file> <output_file> <field> <k>
```

**Esempi di utilizzo:**
```bash
# Ordina per campo id con k=50 (ottimale)
$ ./bin/mainEx1 data/records.bin data/sorted_id_k50.bin 1 50

# Ordina per campo field1 usando solo QuickSort
$ ./bin/mainEx1 data/records.bin data/sorted_field1.bin 2 0

# Ordina per campo field3 (stringhe) con k=100
$ ./bin/mainEx1 data/records.bin data/sorted_field3.bin 4 100
```

---

## 👥 Autori

[Simone Pollano] – 1068650 – simone.pollano593@edu.unito.it