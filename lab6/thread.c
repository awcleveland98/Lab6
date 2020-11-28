//thread.c
#include "queue.h"
#include "testpbthreads.h"
#include "thread.h"
#include "keygen.h"

unsigned char  s[256];
unsigned char  t[256];
unsigned char  key[256];

void swap( unsigned char *p1, unsigned char *p2){
    unsigned char t = *p1;
    *p1 = *p2;
    *p2 = t;
}

void rc4_init( unsigned char *key, int key_len){
    int   i,j=0;
    char* k=key;
    //Initial values of both vectors
    for (i=0;i<256;i++){
        s[i] = i;
        t[i] = k[i%key_len];
    }
    //Initial permutation 
    for (i=0;i<256;i++){
        j = (j + s[i] + t[i])%256;
        swap(&s[i],&s[j]);
    }
}

void rc4( unsigned char* key,  int key_len, char* buff, int len){   

    pthread_mutex_lock(&task_mutex);
    rc4_init(key,key_len);
    //process a byte at a time
    unsigned long t1=0,t2=0;
    for (int i=0;i<len;i++){     
        t1 = (t1 + 1)%256;
        t2 = (t2 + s[t1])%256;
        swap(&s[t1],&s[t2]);
        unsigned char val = (s[t1] + s[t2])%256;
        unsigned char out = *buff ^ val;
        *buff=out;        
        buff++;        
   }
   pthread_mutex_unlock(&task_mutex);
}

void *work(void *arg)
{
    Thread *tskcur = arg;
    TQueue *queue = tskcur->queue;
    Task *t;
    unsigned char key[256], message[256], originalmessage[256];
    int keylen, meslen;

    FILE *file = fopen("message.txt", "rb");
		fgets(originalmessage, 256, (FILE*)file);
		fclose(file);

    while (1) {
        t = dequeue(queue);

        if (t == NULL)
            break;
        
        for (int i = 0; i < 256; i++)
        {
            if(t->key[i] != 0)
                key[i] = t->key[i];
        }
        for (int i = 0; i < 256; i++)
        {
            if(t->message[i] == 0)
                break;
            
            message[i] = t->message[i];
        }
        
        keylen = sizeof(key);
        meslen = strlen(message);

        rc4(key, keylen, message, meslen);

        printf("Message Printed by thread: %d\n", tskcur->xid);
        for (int i = 0; i < meslen; i++){
            printf("%c", message[i]);
        }

        printf("\n\n");

        if (strcmp(originalmessage, message) == 0)
        {
            printf("Foumd it");
            break;
        }
    }
    return (void *) 0;
}

void threads(unsigned int *tkey, char *sBuff){
    TQueue *queue;
    Task *t;
    Thread *tsk;
    unsigned size = 1;

    queue = malloc(sizeof(TQueue));
    startQueue(queue);

    if (opt_T == 0)
        opt_T = 10;
    Thread threads[opt_T];

    if (opt_Q == 0)
        opt_Q = 10000;
    t = malloc(sizeof(Task) * opt_Q);

    for (int i = 0; i < opt_Q; i++) {
        keygen(&tkey, &size, 1);
        t[i].key = tkey;
        t[i].message = sBuff;
        enqueue(queue, &t[i]);
    }

    for (int i = 0; i < opt_T; i++) {
        tsk = &threads[i];
        tsk->xid = i + 1;
        tsk->queue = queue;
        pthread_create(&tsk->tid, NULL, work, tsk);
    }

    for (int i = 0; i < opt_T; i++) {
        tsk = &threads[i];
        pthread_join(tsk->tid, NULL);
    }

    free(t);
}
