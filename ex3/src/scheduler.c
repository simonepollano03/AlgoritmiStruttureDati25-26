#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    #include "../include/scheduler.h"
    #include "../include/priorityQueue.h"

#define MAX_LINE_LENGTH 256
#define INITIAL_TASK_CAPACITY 100

/**
 * @brief Funzione di confronto per i task basata sulla priorità
 * 
 * Task con priorità maggiore hanno precedenza.
 * In caso di parità di priorità, si potrebbe usare l'ID come tie-breaker.
 */
static int task_compare_priority(const void* a, const void* b) {
    const Task* task_a = (const Task*)a;
    const Task* task_b = (const Task*)b;

    return (task_a->priority) - (task_b->priority);
}

/**
 * @brief Funzione di hash per i task (basata sull'ID)
 */
static unsigned long task_hash(const void* element) {
    const Task* task = (const Task*)element;
    
    return (unsigned long)task->id;
}

/**
 * @brief Funzione di confronto per uguaglianza tra task (basata su ID)
 */
static int task_compare_equal(const void* a, const void* b) {
    const Task* task_a = (const Task*)a;
    const Task* task_b = (const Task*)b;

    return (task_a->id) - (task_b->id);
}

/**
 * @brief Stampa un task su stdout (per debug)
 */
static void print_task(const Task* task) {
    if (task == NULL) {
        fprintf(stderr, "ERROR - Task: NULL\n");
        return;
    }
    printf("Task[ ID=%d | Start=%d | Length=%d | Priority=%d ]\n",
            task->id, task->start, task->length, task->priority);
}

/**
 * @brief Stampa un risultato su stdout (per debug)
 */
static void print_result(const TaskResult* result) {
    if (result == NULL) {
        fprintf(stderr, "ERROR - Result: NULL\n");
        return;
    }
    printf("Result [ ID=%d | StartTime=%d | EndTime=%d ]\n",
           result->id, result->start_time, result->end_time);
}


int load_tasks_from_csv(const char* file_name, Task** tasks, int* count) {
    if (file_name == NULL || tasks == NULL || count == NULL) return -1;

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR - Impossibile aprire file '%s'\n", file_name);
        return -1;
    }

    int capacity = INITIAL_TASK_CAPACITY;
    *tasks = (Task*)malloc(capacity * sizeof(Task));
    if (*tasks == NULL) {
        fclose(file);
        fprintf(stderr, "ERROR - malloc fallita\n");
        return -1;
    }

    *count = 0;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;

        // Rimuovi newline
        line[strcspn(line, "\n\r")] = '\0';

        // Salta righe vuote
        if (strlen(line) == 0) continue;

        // Espandi l'array se necessario
        if (*count >= capacity) {
            capacity *= 2;

            Task* new_tasks = (Task*)realloc(*tasks, capacity * sizeof(Task));
            if (new_tasks == NULL) {
                free(*tasks);
                *tasks = NULL;
                fclose(file);
                return -1;
            }

            *tasks = new_tasks;
        }

        // Parse della riga csv
        Task* current_task = &(*tasks)[*count];
        int parsed = sscanf(line, "%d,%d,%d,%d",
                            &current_task->id,
                            &current_task->start,
                            &current_task->length,
                            &current_task->priority);

        if (parsed != 4) {
            fprintf(stderr, "ERROR - formato non valido in rgia %d: '%s'\n", line_number, line);
            continue;
        }

        (*count)++;
    }

    fclose(file);
    return 0;
}

int save_results_to_csv(const char* file_name, const TaskResult* results, int count) {
    if (file_name == NULL || results == NULL || count < 0) return -1;

    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR - impossibile creare il file '%s'\n", file_name);
        return -1;
    }

    // Scrivi header
    fprintf(file, "ID,StartTime,EndTime\n");

    // Scrivi i risultati
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%d,%d\n",
                results[i].id,
                results[i].start_time,
                results->end_time);
    }

    fclose(file);
    return 0;
}

int run_scheduler(const Task* tasks, int task_count, TaskResult* results) {
    if (tasks == NULL || task_count <= 0 || results == NULL) return -1;

    // Crea la priority queue
    PriorityQueue* pq = priority_queue_create(task_compare_priority, task_hash);
    if (pq == NULL) return -1;

    int current_time = 0;           /* Tempo corrente della simulazione */
    int next_task_index = 0;        /* Prossimo task da considerare (ordinati per start) */
    int executed_count = 0;         /* Numero di task eseguiti */

    // Continua fino a che non ci sono task da eseguire
    while (executed_count < task_count) {

        //............
    }

}