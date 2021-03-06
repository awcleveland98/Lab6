#ifndef THREAD_H
#define THREAD_H

#include <string.h>
#include <unistd.h>

void swap( unsigned char *p1, unsigned char *p2);

void rc4_init( unsigned char *key, int key_len);

void rc4( unsigned char* key,  int key_len,
                   char* buff, int len);

void *work(void *arg);

void threads(unsigned int *tkey, char *sBuff);

#endif

