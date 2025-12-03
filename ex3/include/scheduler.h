#ifndef SCHEDULER_H
#define SCHEDULER_H
    
    #include <stdio.h>

    /**
     * @brief Struttura che rappresenta un task da schedulare
     */
    typedef struct task {
        int id;         /**< Identificatore univoco del task */
        int start;      /**< Tempo di arrivo nel sistema */
        int length;     /**< Durata di esecuzione */
        int priority;   /**< Priorità (più alto = più urgente) */
    } Task;

    /**
     * @brief Struttura che rappresenta il risultato dell'esecuzione di un task
     */
    typedef struct task_result {
        int id;         /**< Identificatore del task */
        int start_time; /**< Tempo effettivo di inizio esecuzione */
        int end_time;   /**< Tempo di completamento */
    } TaskResult;

    /**
     * @brief Carica i task da un file CSV
     * 
     * Il file deve avere il formato: ID,Start,Length,Priority
     * I task devono essere già ordinati per tempo di inizio.
     * 
     * @param filename Percorso del file CSV
     * @param tasks Array di task (output, allocato dalla funzione)
     * @param count Numero di task caricati (output)
     * @return int 0 in caso di successo, -1 in caso di errore
     */
    int load_tasks_from_csv(const char* file_name, Task** tasks, int* count);

    /**
     * @brief Salva i risultati della simulazione su file CSV
     * 
     * Il file avrà il formato: ID,StartTime,EndTime
     * 
     * @param filename Percorso del file CSV di output
     * @param results Array dei risultati
     * @param count Numero di risultati
     * @return int 0 in caso di successo, -1 in caso di errore
     */
    int save_results_to_csv(const char* file_name, const TaskResult* result, int count);

    /**
     * @brief Esegue la simulazione dello scheduler
     * 
     * Simula un esecutore singolo non-preemptive che:
     * 1. Sceglie il task con priorità massima tra quelli già arrivati
     * 2. Esegue il task fino al completamento
     * 3. Ripete finché tutti i task sono stati eseguiti
     * 
     * @param tasks Array dei task da schedulare (ordinati per tempo di arrivo)
     * @param task_count Numero di task
     * @param results Array dei risultati (output, allocato dalla funzione)
     * @param result_count Numero di risultati (output)
     * @return int 0 in caso di successo, -1 in caso di errore
     */
    int run_scheduler(const Task* tasks, int task_count, TaskResult** results, int* result_count);

    /**
     * @brief Funzione di confronto per i task basata sulla priorità
     * 
     * @param a Primo task
     * @param b Secondo task
     * @return int Positivo se a ha priorità maggiore, negativo se b ha priorità maggiore
     */
    int task_compare_priority(const void* a, const void* b);

    /**
     * @brief Funzione di hash per i task
     * 
     * @param task Puntatore al task
     * @return unsigned long Valore hash
     */
    unsigned long task_hash(const void* task);

    /**
     * @brief Funzione di confronto per uguaglianza tra task (basata su ID)
     * 
     * @param a Primo task
     * @param b Secondo task
     * @return int 0 se uguali, diverso da 0 altrimenti
     */
    int task_compare_equal(const void* a, const void* b);

    /**
     * @brief Libera la memoria allocata per un array di task
     * 
     * @param tasks Array di task da liberare
     */
    void free_tasks(Task* tasks);

    /**
     * @brief Libera la memoria allocata per un array di risultati
     * 
     * @param results Array di risultati da liberare
     */
    void free_results(TaskResult* results);

    /**
     * @brief Stampa task su stdout (per debug)
     */
    void print_task(const Task* task);

    /**
     * @brief Stampa risultato su stdout (per debug)
     */
    void print_result(const TaskResult* result);
    
#endif