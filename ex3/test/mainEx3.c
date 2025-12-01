#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
    #include "../include/scheduler.h"

int main (int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
        fprintf(stderr, "  input_file:  file CSV con i task (ID,Start,Length,Priority)\n");
        fprintf(stderr, "  output_file: file CSV per i risultati (ID,StartTime,EndTime)\n");
        return EXIT_FAILURE;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║           SCHEDULER NON-PREEMPTIVE (EX3)               ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");

    // Carica i task dal csv
    printf("Caricamento task da %s...\n", input_file);

    Task* tasks = NULL;
    int task_count = 0;

    clock_t start_load = clock();
    if (load_tasks_from_csv(input_file, &tasks, &task_count) != 0) {
        fprintf(stderr, "ERROR - Impossibile caricare i task\n");
        return EXIT_FAILURE;
    }
    clock_t end_load = clock();

    printf("Task caricati: %d\n", task_count);
    double load_time = (double)(end_load - start_load) / CLOCKS_PER_SEC * 1000;
    printf("Tempo di caricamento: %.3f ms\n\n", load_time);

    if (task_count == 0) {
        fprintf(stderr, "ERROR - Nessun task trovato nel file\n");
        free(tasks);
        return EXIT_FAILURE;
    }

    // DEBUG
    // Mostra alcuni task
    printf("Primi task (max 5):\n");
    for (int i = 0; i < task_count && i < 5; i++) {
        printf("  ");
        print_task(&tasks[i]);
    }
    if (task_count > 5) {
        printf("   ... e altri %d task\n", task_count - 5);
    }
    printf("\n");
    // END DEBUG

    // Alloca array per i risultati
    TaskResult* results = (TaskResult*)malloc(task_count * sizeof(TaskResult));
    if (results == NULL) {
        fprintf(stderr, "ERROR - Impossibile allocare memoria per i risultati\n");
        free(tasks);
        return EXIT_FAILURE;
    }

    // Eseguo scheduling
    printf("Esecuzione scheduling...\n");

    TaskResult* results = NULL;
    int executed = 0;

    clock_t start_sched = clock();
    int ret = run_scheduler(tasks, task_count, &results, &executed);
    clock_t end_sched = clock();

    if (ret != 0 || results == NULL) {
        fprintf(stderr, "ERROR - Errore durante lo scheduling\n");
        free(tasks);
        return EXIT_FAILURE;
    }

    double sched_time = (double)(end_sched - start_sched) / CLOCKS_PER_SEC * 1000;
    printf("Task eseguiti: %d\n", executed);
    printf("Tempo di scheduling: %.3f ms\n\n", sched_time);

    // DEBUG
    // Mostra alcuni risultati
    printf("Primi risultati (max 5):\n");
    for (int i = 0; i < executed && i < 5; i++) {
        printf("  ");
        print_task(&results[i]);
    }
    if (executed > 5) {
        printf("   ... e altri %d risultati\n", executed - 5);
    }
    printf("\n");
    // END DEBUG

    // Calcola statistiche
    if (executed > 0) {
        int total_wait_time = 0;
        int max_wait_time = 0;

        for (int i = 0; i < executed; i++) {
            for (int j = 0; j < task_count; j++) {
                if (tasks[j].id == results[i].id) {
                    int wait_time = results[i].start_time - tasks[j].start;
                    total_wait_time += wait_time;

                    if (wait_time > max_wait_time) {
                        max_wait_time = wait_time;
                    }

                    break;
                }
            }
        }

        printf("Statistiche:\n");
        printf("  Tempo medio di attesa: %.2f\n", (double)total_wait_time / executed);
        printf("  Tempo massimo di attesa: %d\n", max_wait_time);
        printf("  Tempo finale simulazione: %d\n\n", results[executed - 1].end_time);
    }

    // Salva i risultati
    printf("Salvataggio risultati in '%s'...\n", output_file);

    if (save_results_to_csv(output_file, results, executed) != 0) {
        fprintf(stderr, "ERROR - Impossibile salvare i risultati\n");
        free(tasks);
        free(results);
        return EXIT_FAILURE;
    }

    printf("Risultati salvati con successo!\n");

    // Cleanup
    free(tasks);
    free(results);

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║                    COMPLETATO                          ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}