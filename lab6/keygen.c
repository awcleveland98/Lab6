#include "keygen.h"

void keygen(unsigned char** currentKey, unsigned* outputSize, unsigned increment) {
    unsigned index = 0;
    for (int i = 0; i < increment; ++i) {
        while (++(*currentKey)[index] == 0) {
            if (++index == *outputSize) {
                free(*currentKey);
                *currentKey = calloc(1, ++*outputSize);
            }
        }
    }

}
