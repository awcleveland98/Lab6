#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "queue.h"
#include "keygen.h"

int main(int argc,char ** argv){  
	char sBuff[256];
	int thrs = 0;
	if (argc > 1) {
		FILE *file = fopen(argv[1], "rb");
		fgets(sBuff, 256, (FILE*)file);
		fclose(file);
	}
	if (argc > 2) {
		thrs = atoi(argv[2] + 2);
	}
	if (argc > 3) {
		fprintf(stderr, "Usage: %s [Encoded Message] [Number of Threads]\n", argv[0]);
		abort();
	}
	
	printf("Encoded Message: ");
	int sBuff_len = strlen(sBuff);
	for (int i=0;i<sBuff_len;i++)
		printf("%c", sBuff[i]);

	printf("\n");
	
	
    unsigned int* tkey = malloc(sizeof(unsigned int) * 64);
    tkey[0] = 0;

	threads(tkey, sBuff);
}

