## Relazione Esercizio 2

---

## Struttura della directory

`ex2/`  
├── `src/`               # Codici sorgente (.c) per l'hashTable  
├── `include/`           # Files header (.h)  
├── `test/`              # Codice di test con Unity framework e main  
├── `bin/`               # Files eseguibili (generati da make)  
├── `docs/`              # Documentazione generata (HTML)  
├── `Makefile`           # Build automation  
├── `Doxyfile`           # Configurazione Doxygen    
└── `README2.md`          # Questo file  

---

## Implementazione

### Funzioni principali implementate

#### 1. Hash Table con concatenamento
```c
HashTable* hash_table_create(int (*compare)(const void*,const void*),
                             unsigned long (*hash)(const void*));
```

- **Gestione collisioni**: concatenamento tramite liste collegate
- **Ridimensionamento automatico**: capacità raddoppiata quando load factor > 0.75
- **Genericità**: supporta chiavi e valori di qualsiasi tipo tramite `void*`
- **Complessità**: O(1) medio per `put()`, `get()`, `remove()`

#### 2. Operazioni sull'hash table
```c
void hash_table_put(HashTable*, const void* key, const void* value);
void* hash_table_get(const HashTable*, const void* key);
int hash_table_contains_key(const HashTable*, const void* key);
void hash_table_remove(HashTable*, const void* key);
int hash_table_size(const HashTable*);
void** hash_table_keyset(const HashTable*);
void hash_table_free(HashTable*);
```

- **Inserimento**: aggiorna il valore se la chiave esiste, altrimenti inserisce nuova entry
- **Ricerca**: O(1) medio tramite funzione hash
- **Rimozione**: mantiene integrità delle liste concatenate
- **Keyset**: restituisce array di tutte le chiavi per iterazioni

#### 3. Applicazione: Conteggio delle parole
```c
static int process_file(const char* filename, HashTable* table);
static char* find_most_frequent_word(HashTable* table, size_t min_length);
```

- **Preprocessing**: rimozione caratteri non alfabetici e conversione a minuscolo
- **Conteggio**: chiavi=parole, valori=contatori di frequenza
- **Ricerca**: scansione lineare del keyset con filtro su lunghezza minima

#### 4. Funzioni hash
```c
static unsigned long string_hash(const void* key);
static int string_compare(const void* a, const void* b);
static void to_lowercase(char* str);
static void clean_word(char* word);
```

- `string_hash()`: implementa l'**algoritmo djb2** per generare hash delle stringhe
- `string_compare()`: wrapper di `strcmp()` per compatibilità con l'interfaccia generica
- `to_lowercase()`: converte una stringa in minuscolo
- `clean_word()`: rimuove caratteri non alfabetici dalle parole


---

## Processo di sviluppo con LLM

### Approccio utilizzato

Per questo esercizio è stato utilizzato ***Claude PRO (Anthropic)*** come Large Language Model per supportare lo sviluppo dell'implementazione.

### Iterazioni principali

#### 1. Propmt iniziale
Come prompt iniziale, è stato fornito al modello il file `README.md` presente nel repository GitLab del laboratorio, specificando l'obiettivo di implementare una **tabella hash generica in linguaggio C**.  

È inoltre stato richiesto al modello di **leggere attentamente e rispettare tutte le regole e specifiche indicate nel progetto**, obbligandolo a seguire fedelmente le direttive fornite prima di generare qualsiasi parte di codice o suggerimento.  

In questo modo, l'assistenza dell'LLM è stata guidata da vincoli precisi, mantenendo la coerenza con le linee guida del laboratorio.

#### Output iniziale
- Struttura base corretta con liste concatenate
- Implementazione di `_create()`, `_put()` funzionanti
- Problema 1: bug critico nella funzione `_remove()`
- Problema 2: bug critico nella funzione `_get()`

#### Analisi critica - Bug in `_remove()`
```c
void hash_table_remove(HashTable* table, const void* key) {
    // ... calcolo hash e index ...
        
    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {
            table->buckets[index] = current->next;  // ERRORE!
        } else {
            prev->next = current->next;  // Crash se prev è NULL!
        }
        free(current);
        table->size--;
        return;
    }
    prev = current;          // Fuori dal while, mai eseguito!
    current = current->next;
}
```
**Problemi identificati:**
- Non distingue tra la rimozione del primo nodo e dei nodi successivi
- Accede a `prev->next` quando `prev == NULL` causando un segmentation fault
- L'avanzamento della lista è fuori dal `while` generando un loop infinito
- Rimuove sempre ed unicamente il primo nodo, indipendentemente dalla chiave

#### Analisi critica - Bug in `_get()`
```c
void* hash_table_get(const HashTable* table, const void* key) {
    // ... calcolo hash e index ...

    while (table->compare(current->key, key) == 0) {  // ERRORE!
        return current->value;
    }
    
    return NULL;
}
```
**Problemi identificati:**
- La condizione del `while(_)` è errata: confronta solo il primo elemento
- Se il primo elemento è uguale, restituisce il valore
- Se il primo elemento è diverso, esce dal `while` senza scorrere la lista e di conseguenza senza controllare gli elementi successivi
- Manca l'avanzamento: `current = current->next;`

#### 3. Raffinamento del prompt
Per correggere i due problemi riscontrati, sono stati copiati i messaggi di errore dal terminale e forniti al modello all'interno di un prompt più mirato.  

In particolare, è stato specificato:
- Nella funzione `_remove()`, è presente un bug riguardante la mancata distinzione tra il caso del primo nodo e gli altri, e che le variabili `prev` e `current` devono essere aggiornate all'interno del ciclo `while`.
- Nella funzione `_get()`, la condizione del ciclo è errata e deve essere `while(current != NULL)`

#### 4. Unit testing in `testEx2.c` con LLM
**Prompt**:  
"Genera unit test completi per la hash table coprendo le diverse funzioni di creazione, inserimento singolo/multiplo, aggiornamento, get con chiavi inesistenti, remove con collisioni, keyset, casi con NULL, stress test con 1000 elementi"

**Risultato**:  
- Vari test case, in formato Unity, generati automaticamente
- Copertura completa di tutte le funzioni
- **Problema**: Alcuni test usavano macro Unity errate...

```c
// Generato da Claude:
RUN_UPDATE(test_hash_table_put_update);

// Corretto manualmente
RUN_TEST(test_hash_table_put_update);

```

#### 5. Implementazione `mainEx2.c` con LLM
Per quanto riguarda il file `mainEx2.c`, l’LLM è stato in grado di generarlo automaticamente, senza la necessità di fornire un prompt specifico, producendo un codice funzionante e privo di errori.

---

## Analisi dei risultati
Sono state testati alcuni valori di `lunghezza_minima` per verificare la correttezza.
<div align="center">

| **Lunghezza minima** | **Parola trovata** | **Frequenza** |
|:--------------------:|:------------------:|:-------------:|
| 3                    | the                | 9967          |
| 5                    | their              | 872           | 
| 6                    | trojans            | 562           |
| 10                   | themselves         | 66            |
| 16                   | multitudedeathcry  | 1             |

</div>

---

## Considerazioni sull'uso degli LLM
Gli LLM rappresentano uno strumento estremamente utile per supportare l’attività di programmazione, poiché possono assistere nello sviluppo del codice, nella correzione di errori e nella generazione di soluzioni alternative.  
Tuttavia, il loro utilizzo richiede cautela e consapevolezza.  

È importante, innanzitutto, scegliere il modello più adatto al tipo di compito da svolgere e, in secondo luogo, mantenere un atteggiamento critico e di supervisione verso ciò che viene prodotto.  

Un altro aspetto fondamentale è la capacità di scrivere prompt chiari e mirati, fornendo al modello tutte le informazioni necessarie, in altre parole, “dargli la pappa pronta”; affinché il risultato sia coerente e di qualità.  

Senza dubbio, gli LLM stanno già cambiando il modo di programmare, ma il loro impiego efficace dipende dalla capacità dell’utente di saperli guidare, interpretare e controllare.


---

## Compilazione ed Esecuzione

### Compilazione

Il progetto utilizza un Makefile per automatizzare la compilazione. I comandi disponibili sono:
```bash
# Compila tutti gli eseguibili (mainEx2 e testEx2)
make all

# Pulisce i file compilati
make clean

# Compila ed esegue i test automatici
make test

# Stampa la sintassi di utilizzo dell'esercizio 
make howto
```

### Esecuzione

#### 1. Test automatici

Per verificare la correttezza dell'implementazione:
```bash
$ make test
# oppure
$ ./bin/testEx2
```

#### 2. Trova parola più frequente

**Sintassi:**
```bash
./bin/mainEx2 <percorso_file> <lunghezza_minima>
```

**Esempi di utilizzo:**
```bash
# Parola più frequente con lunghezza >= 3
$ ./bin/mainEx2 data/iliade.txt 3
# Parola più frequente con lunghezza >= 10
$ ./bin/mainEx2 data/iliade.txt 10
```

## 👥 Autori

[Simone Pollano] – 1068650 – simone.pollano593@edu.unito.it


