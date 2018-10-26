#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pcg_basic.h>

#define RAND32_MAX 4294967295 // 2**32 - 1

float pcgrand(pcg32_random_t *rng){
    return pcg32_random_r(rng) * (1.0 / RAND32_MAX);
}

float pcgrandn(pcg32_random_t *rng){
    float U1, U2, W=0, mult;
    static float X1, X2;  // save these values between calls!
    static int call = 0;

    if (call == 1){
        call = !call;
        return (float) X2;
    }

    while (W >= 1 || W == 0){
        U1 = 2*pcgrand(rng)-1;
        U2 = 2*pcgrand(rng)-1;
        W = U1*U1 + U2*U2;
    }

    mult = sqrt((-2 * log(W))/W);
    X1 = U1 * mult;
    X2 = U2 * mult;
    call = !call;
    return X1;
}

void printSpins(int *s, int L) {
    for (int i=0; i < L; i++){
        for (int j=0; j < L; j++){
            printf("%i ", s[i*L + j]);
        }
        printf("\n");
    }
}
