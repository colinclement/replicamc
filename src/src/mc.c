#include <stdlib.h>
#include <math.h>
#include <pcg_basic.h>
#include <cluster.h>
#include <graph.h>
#include <ll.h>

#define RAND32_MAX 4294967295 // 2**32 - 1

float pcgrand(pcg32_random_t *rng){
    return (float) pcg32_random_r(rng) / (float) RAND32_MAX;
}

void mhsweep(int *s, float b, float *J, int L, pcg32_random_t *rng){
    for (int i=0; i < L*L; i++){
        float dE = deltaEsquarelattice(i, s, J, L);
        if (dE < 0)
            s[i] *= -1;
        else if ( exp(-b*dE) > pcgrand(rng) )
            s[i] *= -1;
    }
}

void swstep(int *s1, int s2*, float b, float *J, int L, pcg32_random_t *rng){

}
