#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <cluster.h>
#include <pcg_basic.h>
#include <mc.h>
#include <ll.h>
#include <util.h>

// 2**32-1
#define RAND32_MAX 4294967295

int main(int argc, const char* argv[])
{
    //graph *G0 = initGraph(3);    
    //insertEdge(0, 1, 1, G0, 0);
    //insertEdge(1, 2, 1, G0, 0);
    //insertEdge(2, 0, 1, G0, 0);
    //printGraph(G0);
    //addWeight(0, 1, .1, G0, 0);
    //printGraph(G0);
    //addWeight(0, 2, 1.1, G0, 0);
    //addWeight(0, 2, 1., G0, 0);
    //printGraph(G0);
    //destroyGraph(G0);
    //int s[3] = {1, -1, -1};
    //printf("%f\n", deltaE(0, s, G));
    //destroyGraph(G);

    //ll *list = initList(0);
    //pushint(1, &list);
    //pushint(2141, &list);
    //pushint(4, &list);
    //printll(list);
    //printf("is 0 in the list? %i\n", search(0, list));
    //printf("is 1 in the list? %i\n", search(1, list));
    //printf("is 5 in the list? %i\n", search(5, list));
    //printf("is 2141 in the list? %i\n", search(2141, list));
    //ll *list2 = initList(102);

    //pushint(1230, &list2);

    //lol *lolists = initListofLists(list);
    //pushList(list2, &lolists);
    //printlol(lolists);

    //ll *list3 = initList(102);
    //pushint(1230, &list3);
    //pushint(10, &list3);
    ////int popped = popint(&list3);
    ////printf("popped %i\n", popped);
    //printf("printing list3\n");
    //printll(list3);
    ////popint(&(list3->next->next));
    //removeval(10, &list3);
    //removeval(1230, &list3);
    //removeval(102, &list3);
    //printf("list3 is null %i\n", (list3==NULL));
    //printf("printing list3\n");
    //printll(list3);

    //destroyListofLists(lolists);
    //destroyList(list3);
    int L=4;
    int s[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        -1,1,1,1,1,1,-1,1,1,-1,-1,1,1,1,1,1};

    float b[2] = {.483, .404};
    int Nb = 2;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, 42u, 54u); // seed deterministically

    printf("s1\n");
    printSpins(s, L);
    printf("s2\n");
    printSpins(s+L*L, L);

    float *J = (float *)malloc(2*L*L*sizeof(float));
    for (int i=0; i < 2*L*L; i++)
        J[i] = 1.; 

    //int *label = (int *)malloc(L*L*sizeof(int)), Nc;
    //graph *G;
    //clusterGraph(s1, s2, J, L, &label, &G);
    //printSpins(label, L);
    //printGraph(G);

    //int flip[3] = {-1, -1, -1};
    //flipClusters(s1, s2, L, flip, label);
    //
    mhsweep(s, b, Nb, J, L, &rng);

    printf("s1\n");
    printSpins(s, L);
    printf("s2\n");
    printSpins(s+L*L, L);

    swstep(s, b, Nb, J, L, &rng);

    printf("s1\n");
    printSpins(s, L);
    printf("s2\n");
    printSpins(s+L*L, L);

    //free(label);
    //destroyGraph(G);
    free(J);

    //pcg32_random_t rng;
    //pcg32_srandom_r(&rng, 42u, 54u); // seed deterministically
    //for (int i=0; i < 100; i++)
    //    printf("%f\n ", ((float)pcg32_random_r(&rng)) / (float) RAND32_MAX);
    //printf("\n");
    //printf("%li \n", RAND32_MAX);
    return 0;
}
