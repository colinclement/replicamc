#include <stdlib.h>
#include <assert.h>
#include <pcg_basic.h>
#include <cluster.h>
#include <graph.h>
#include <ll.h>
#include <math.h>
#include <util.h>

void randomize(int *s, int Nb, int L, pcg32_random_t *rng){
    assert(NULL != s);
    assert(Nb > 0);
    assert(L > 0);
    assert(NULL != rng);

    for (int i=0; i < Nb*L*L; i++)
        s[i] = (pcgrand(rng) < 0.5) ? -1 : 1;
}

void mhstep(int *s, float b, float *J, int L, pcg32_random_t *rng){
    assert(NULL != s);
    assert(b > 0);
    assert(NULL != J);
    assert(L > 0);
    assert(NULL != rng);

    for (int i=0; i < L*L; i++){
        float dE = deltaEsquarelattice(i, s, J, L);
        if (dE < 0)
            s[i] *= -1;
        else if ( exp(-b*dE) > pcgrand(rng) )
            s[i] *= -1;
    }
}

void mhsweep(int *s, float *b, int Nb, float *J, int L, pcg32_random_t *rng){
    assert(NULL != s);
    assert(NULL != b);
    assert(Nb > 0);
    assert(NULL != J);
    assert(L > 0);
    assert(NULL != rng);

    for (int i=0; i < Nb; i++)
        mhstep(s+i*L*L, b[i], J, L, rng);
}

void swstep(int *s, float *b, int Nb, float *J, int *label, int L, 
        pcg32_random_t *rng){
    assert(NULL != s);
    assert(NULL != b);
    assert(Nb > 0);
    assert(NULL != J);
    assert(NULL != label);
    assert(L > 0);
    assert(NULL != rng);

    int N = L*L;
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
}
