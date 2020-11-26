//thread.c
#include "./queue.h"
#include "./testpbthreads.h"
#include "./thread.h"

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

void rc4( unsigned char* key,  int key_len,
                   char* buff, int len){   
    
    
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
}

void threads(){
    char *cp;
    TQueue *queue;
    Task *t;
    Thread *tsk;

    FILE *file2 = fopen("/dev/random","r");
    int key_len = 10;
    unsigned char tkey[ key_len ];
    for (int i = 0; i < opt_Q; i++)
        fread(tkey, key_len, 1, file2);

    tvzero = tvgetf();

    queue = malloc(sizeof(TQueue));
    startQueue(queue);

    if (opt_T == 0)
        opt_T = 1;
    Thread threads[opt_T];

    if (opt_Q == 0)
        opt_Q = 100;
    t = malloc(sizeof(Task) * opt_Q);

    for (int i = 0; i < opt_Q; i++) {
        strcpy(t[i].key, tkey);
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

    printf("TOTAL: %.9f\n",tvgetf());

    free(t);
}
