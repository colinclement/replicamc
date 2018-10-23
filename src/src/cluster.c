#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>
#include <ll.h>


void swendsenWangCluster(int *s1, int *s2, int **label, int *Nc, lol **outbounds, int L) {
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
    printf("starting clustering\n");
    *Nc = 0;
    while (spins != NULL) {
        // try making a new cluster starting with seed
        int seed = popint(&spins);
        (*label)[seed] = *Nc;
        ll *queue = initList(seed);
        ll *outb = initList(-1); // -1 indicates empty outerboundary

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
                    } else if (!search(n, outb))
                        pushint(n, &outb);
                } 
            }
        }
        destroyList(queue);
        removeval(-1, &outb);  // since -1 is not a valid index

        if (*Nc == 0)
            *outbounds = initListofLists(outb);
        else
            pushList(outb, outbounds);
        *Nc += 1;
    }

    destroyList(spins);
    free(tau);
}

void clusterGraph(int *s1, int *s2, float *J, graph **G, int L)
{
    int *label, Nc;
    lol *outbounds;
    swendsenWangCluster(s1, s2, &label, &Nc, &outbounds, L);
    *G = initGraph(Nc);
    
    lol *bound = outbounds;
    int i=0;
    while (bound != NULL){
        for (int j=i; j < Nc; j++){
            if (j > i){
                float bond = 0.;
                // iterate over boundaries of one, see if they have neighbors in
                // the other
            }
        }
        i += 1;
        bound = bound->next;
    }
    destroyListofLists(outbounds);
}
