#include <stdlib.h>
#include <graph.h>
#include <ll.h>

#define idx(y, x, L) (y*L + x)
#define neighbors(y, x, L) {idx(y, ((x-1)%L), L),\
                            idx(y, ((x+1)%L), L),\
                            idx(((y-1)%L), x, L),\
                            idx(((y+1)%L), x, L)}
#define coordination 4

void swendsenwangcluster(int *s1, int *s2, lol **clusters, lol **outbounds, int L) {
    int N = L*L;
    int *tau = (int *)malloc(N*sizeof(int));
    ll *spins = initList(0);

    for (int i=0; i < N; i++) {
        tau[i] = s1[i] * s2[i];
        if (i > 0)
            pushint(i, &spins);
    }

    int Nc = 0;
    while (spins != NULL) {
        int seed = popint(&spins);
        ll *clust = initList(seed);
        ll *queue = initList(seed);
        ll *outb = initList(seed);

        while (queue != NULL) {
            int next = popint(&queue);
            int neighs[] = neighbors(next/L, next%L, L);
            for (int i=0; i < coordination; i++) {
                int n = neighs[i];
                if (search(n, clust))  // ignore sites in clust
                    continue;
                else if (tau[seed] == tau[n]) {
                    pushint(n, &clust);
                    pushint(n, &queue);
                    removeval(n, &spins);
                } else {
                    pushint(n, &outb);
                }
            }
        }

        if (Nc == 0) {  // initialized clusters have trivial lists
            (*clusters)->list = clust;
            (*outbounds)->list = outb;
        } else {
            pushList(clust, clusters);
            pushList(outb, outbounds);
        }   
        Nc += 1;
    }
    free(tau);
}
