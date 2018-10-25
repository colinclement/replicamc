#include <stdlib.h>
#include <pcg_basic.h>
#include <cluster.h>
#include <graph.h>
#include <ll.h>
#include <math.h>

#define RAND32_MAX 4294967295 // 2**32 - 1

float pcgrand(pcg32_random_t *rng){
    return (float) pcg32_random_r(rng) / (float) RAND32_MAX;
}

void mhstep(int *s, float b, float *J, int L, pcg32_random_t *rng){
    for (int i=0; i < L*L; i++){
        float dE = deltaEsquarelattice(i, s, J, L);
        if (dE < 0)
            s[i] *= -1;
        else if ( exp(-b*dE) > pcgrand(rng) )
            s[i] *= -1;
    }
}

void mhsweep(int *s, float *b, int Nb, float *J, int L, pcg32_random_t *rng){
    for (int i=0; i < Nb; i++)
        mhstep(s+i*L*L, b[i], J, L, rng);
}

void swstep(int *s, float *b, int Nb, float *J, int L, pcg32_random_t *rng){
    int N = L*L;
    int *label=(int *)malloc(N*sizeof(int));
    for (int i=0; i < Nb-1; i++){
        graph *G;
        float b_eff = b[i] - b[i+1];
        int *s1 = s + i*N, *s2 = s + (i+1)*N;
        clusterGraph(s1, s2, J, L, label, &G);

        int *flip = (int *)malloc((G->Nv) * sizeof(int));
        for (int c=0; c < G->Nv; c++)
            flip[c] = 1;

        // Metropolis-Hastings on the cluster Hamiltonian
        for (int c=0; c < G->Nv; c++){
            float dE = deltaE(c, flip, G);
            if (dE < 0)
                flip[c] *= -1;
            else if ( exp(-b_eff*dE) > pcgrand(rng) )
                flip[c] *= -1;
        }
        flipClusters(s1, s2, L, flip, label);

        destroyGraph(G);
        free(flip);
    }
    free(label);
}
