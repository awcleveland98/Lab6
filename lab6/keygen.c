#include "keygen.h"

void keygen(unsigned char tkey[64]){
    
	srand(time(NULL));

	for (int i = 0; i < 64; i++)
	{
		tkey[i] = (unsigned char) rand() % 128;
	}

}
