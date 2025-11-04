#include <stdio.h>
#include <stdlib.h>
    #include "../include/breadthFirstVisit.h"
    #include "../../ex2/include/hashTable.h"

// Struttura per coda fifo
typedef struct queue_node {
    void* data;
    struct queue_node* next;
} QueueNode; 

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

static Queue* queue_create(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    queue->front = NULL;
    queue->rear  = NULL;
    queue->size  = 0;
}

static int queue_is_empty(const Queue* queue) {
    return queue == NULL || queue->front == NULL;
}

// enqueue si intede che inserisce un elemento al fondo alla coda
static int queue_enqueue(Queue* queue, void* data) {
    if (queue == NULL) return 0;

    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) return 0;

    new_node->data = data;
    new_node->next = NULL;

    if (queue_is_empty(queue)) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node; 
    }

    queue->size++;

    return 1;
}


// dequeue si intede che inserisce un elemento all'inizio alla coda
static int queue_dequeue(Queue* queue) {
    if (queue_is_empty(queue)) return NULL;

    QueueNode* temp = queue->front;
    void* data = temp->data;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;

    return data;
}

static void queue_free(Queue* queue) {
    if (queue == NULL) return;

    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }

    free(queue);
}

void** breadth_first_visit(Graph gr, void* start,
                            int (*compare)(const void*, const void*),
                            unsigned long (*hash)(const void*)) {
                                
    if (gr == NULL || start == NULL || 
        compare == NULL || hash == NULL) return NULL;
    
    
}
