#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int opt_T;                              // number of threads
int opt_Q;                              // number of queue items

static pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Task {
    unsigned int *key;
    char *message;
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

void startQueue(TQueue *queue);

int empty(TQueue *queue);

void enqueue(TQueue *queue, Task *t);

Task * dequeue(TQueue *queue);

#endif

