#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>
#include <ll.h>


void swendsenwangcluster(int *s1, int *s2, lol **clusters, lol **outbounds, int L) {
    int N = L*L;
    int *tau = (int *)malloc(N*sizeof(int));
    int *touched = (int *)malloc(N*sizeof(int));
    checkptr(tau)
    checkptr(touched)
    ll *spins = initList(0);

    for (int i=0; i < N; i++) {
        tau[i] = s1[i] * s2[i];
        if (i > 0)
            pushint(i, &spins);
    }

    int Nc = 0;
    // iterate over all the spins
    while (spins != NULL) {
        memset(touched, 0, N*sizeof(int));

        int seed = popint(&spins);
        touched[seed] = 1;
        ll *clust = initList(seed);
        ll *queue = initList(seed);
        ll *outb = initList(-1);

        // try making a new cluster
        while (queue != NULL) {
            int next = popint(&queue);
            int neighs[coordination] = neighbors(next/L, next%L, L);

            for (int i=0; i < coordination; i++) {
                int n = neighs[i];
                if (touched[n])  // ignore sites in clust
                    continue;
                else if (tau[seed] == tau[n]) {
                    touched[n] = 1;
                    pushint(n, &clust);
                    pushint(n, &queue);
                    removeval(n, &spins);
                } else {
                    touched[n] = 1;
                    pushint(n, &outb);
                }
            }
        }
        destroyList(queue);
        removeval(-1, &outb);

        if (Nc == 0) {  // initialized clusters have trivial lists
            *clusters = initListofLists(clust);
            *outbounds = initListofLists(outb);
        } else {
            pushList(clust, clusters);
            pushList(outb, outbounds);
        }   
        Nc += 1;
    }

    destroyList(spins);
    free(tau);
    free(touched);
}
