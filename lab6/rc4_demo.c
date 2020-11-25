/*
 * Cyrus.Sh
 * ~cyn
 * https://github.com/cyrus-sh/RC4
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc,char ** argv){  
    unsigned char message[1000];
    strcpy(message,"Can you please not make assignments political by making Russia a mandatory word.");
    int message_len = strlen(message);
    unsigned char result[1000];
    FILE *f = fopen("/dev/urandom","r");
    int key_len = 10;
    unsigned char key[ key_len ];
    fread(key, key_len, 1, f);

    printf("> Key:          ");
    for (int i=0;i<key_len;i++)
        printf("0x%02X ",key[i]);   //print key in hex
    printf("\n> Plain text:   %s\n",message);
    printf("> Cipher text:  ");
    rc4(key,10,message,message_len);
    for (int i=0;i<message_len;i++) //print ciphertext in hex
        printf("%02X",message[i]);

    printf("\n> Decrypted:    ");
    rc4(key, key_len, message, message_len);
    printf("%s\n",message);
}
