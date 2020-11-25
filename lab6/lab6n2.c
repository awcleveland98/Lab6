/*
 * lab6n2
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
int count = 0;

void* workthread(void *arg) {
   for (int i = 0; i < 20; i++) {
      pthread_mutex_lock( &mutex );
      pthread_mutex_unlock( &mutex );
	count++;
   }
   return (void*) count;
}

int main(int argc, char *argv[]) {
   pthread_t p1, p2;
   printf("Starting 2 threads\n");

   pthread_create(&p1, NULL, workthread, NULL );
   pthread_create(&p2, NULL, workthread, NULL );

   pthread_join( p1, NULL);
   pthread_join( p2, NULL);
   return 0;
}

