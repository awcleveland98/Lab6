//queue.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./testpbthreads.h"

int opt_T;                              // number of threads
int opt_Q;                              // number of queue items

static pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Task {
    unsigned char key[256];
} Task;

typedef struct Cell {
    Task *t;
    struct Cell *next;
} Cell;

typedef struct TQueue {
    struct Cell *head;
    struct Cell *tail;
} TQueue;

typedef struct Thread {
    pthread_t tid;
    int xid;
    TQueue *queue;
} Thread;

void startQueue(TQueue *queue)
{

    queue->head = NULL;
    queue->tail = NULL;
}

int empty(TQueue *queue)
{

    return (queue->head == NULL);
}

void enqueue(TQueue *queue, Task *t)
{
    Cell *p;

    pthread_mutex_lock(&task_mutex);

    p = malloc(sizeof(Cell));
    p->next = NULL;
    p->t = t;

    if (queue->tail == NULL) {
        queue->tail = p;
        queue->head = p;
    }
    else {
        queue->tail->next = p;
        queue->tail = p;
    }

    pthread_mutex_unlock(&task_mutex);
}

Task *dequeue(TQueue *queue)
{
    Task *t;

    pthread_mutex_lock(&task_mutex);

    if (empty(queue))
        t = NULL;

    else {
        Cell *p = queue->head;

        if (p == queue->tail)
            queue->tail = NULL;

        queue->head = p->next;

        t = p->t;

        free(p);
    }

    pthread_mutex_unlock(&task_mutex);

    return t;
}

void *work(void *arg)
{
    Thread *tskcur = arg;
    TQueue *queue = tskcur->queue;
    Task *t;
    double tvbef;
    double tvaft;

    while (1) {
        tvbef = tvgetf();
        t = dequeue(queue);
        tvaft = tvgetf();

        if (t == NULL)
            break;

        printf("[%.9f/%.9f %5.5d]", tvbef,tvaft - tvbef,tskcur->xid);
        
        printf("   %02x", t->key);
          
        printf("\n");
    }

    return (void *) 0;
}

