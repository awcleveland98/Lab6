//queue.c
#include "queue.h"

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

    while (1) {
        
        t = dequeue(queue);

        if (t == NULL)
            break;

        for (int i = 0; i < 256; i++)
        {
            printf("%02x", t->key[i]);
        }
        
        printf("\n\n");

          
    }

    return (void *) 0;
}




