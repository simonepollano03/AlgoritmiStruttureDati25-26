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

static int task_compare_equal(const void* a, const void* b) {
    const Task* task_a = (const Task*)a;
    const Task* task_b = (const Task*)b;

    return (task_a->id) - (task_b->id);
}


static void print_task(const Task* task) {
    if (task == NULL) {
        fprintf(stderr, "ERROR - Task = NULL");
        return;
    }
    printf("Task[ ID=%d | Start=%d | Length=%d | Priority=%d ]\n",
            task->id, task->start, task->length, task->priority);
}

//...........