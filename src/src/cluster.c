#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>
#include <ll.h>


void swendsenWangCluster(int *s1, int *s2, int **label, int *Nc, int L) {
    int N = L*L;
    int *tau = (int *)malloc(N*sizeof(int));
    *label = (int *)malloc(N*sizeof(int));
    checkptr(tau)
    checkptr(*label)
    ll *spins = initList(0);

    for (int i=0; i < N; i++) {
        tau[i] = s1[i] * s2[i];
        (*label)[i] = -1;
        if (i > 0)
            pushint(i, &spins);
    }
    *Nc = 0;
    while (spins != NULL) {
        // try making a new cluster starting with seed
        int seed = popint(&spins);
        (*label)[seed] = *Nc;
        ll *queue = initList(seed);

        while (queue != NULL) {
            int next = popint(&queue);
            int neighs[coordination] = neighbors(next/L, next%L, L);
            // Loop over neighbors of next
            for (int i=0; i < coordination; i++) {
                int n = neighs[i];
                if ((*label)[n] != *Nc){  // if n is not in current cluster 
                    if (tau[seed] == tau[n]){
                        (*label)[n] = *Nc;
                        pushint(n, &queue);
                        removeval(n, &spins);
                    }
                }
            }
        }
        *Nc += 1;
    }
    free(tau);
}

void clusterGraph(int *s1, int *s2, float *J, graph **G, int L)
{
    int *label, Nc;
    swendsenWangCluster(s1, s2, &label, &Nc, L);
    *G = initGraph(Nc);
    for (int x=0; x < L; x++){
        for (int y=0; y < L; y++){
            int s = idx(y, x, L);
            // right and left bonds only to prevent double-counting
            int neighs[] = {idx(y, MOD(x+1, L), L), idx(MOD(y+1,L), x, L)};
            for (int k=0; k < coordination/2; k++){
                int n = neighs[k];
                if ( label[s] != label[n] ){
                    addWeight(label[s], label[n], J[L*L*k + s]*s1[s]*s1[n], *G, 0);
                }
            }
        }
    }
    free(label);
}
