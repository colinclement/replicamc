#include <stdlib.h>
#include <stdio.h>

void printSpins(int *s, int L) {
    for (int i=0; i < L; i++){
        for (int j=0; j < L; j++){
            printf("%i ", s[i*L + j]);
        }
        printf("\n");
    }
}
