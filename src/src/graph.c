#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <ll.h>

graph* initGraph(int Nv) {
    graph *new = (graph *)malloc(sizeof *new);
    new->Nv = Nv;
    new->edges = (edgenode **)malloc(Nv * sizeof(edgenode*));
    checkptr(new)
    for (int i=0; i < Nv; i++)
        *(new->edges + i) = NULL;
    return new;
}

void destroyGraph(graph *G) {
    edgenode *tmp, *head;
    for (int i=0; i < G->Nv; i++) {
        head = *(G->edges + i);
        while (head != NULL) {
            tmp = head;
            head = head->nextedge;
            free(tmp);
        }
    }
    free(G->edges);
    free(G);
}

void printGraph(graph *G) {
    printf("Graph with %i vertices\n", G->Nv);
    edgenode *head;
    for (int i=0; i < G->Nv; i++) {
        head = *(G->edges + i);
        while (head != NULL) {
            printf("%i connected to %i with weight %f\n", i, head->y, head->w);
            head = head->nextedge;
        }
    }
}

void insertEdge(int i, int j, float w, graph *G, int directed) {
    edgenode *new = (edgenode *)malloc(sizeof *new);
    checkptr(new)
    new->y = j;
    new->w = w;
    new->nextedge = *(G->edges+i);
    *(G->edges+i) = new;
    if ( ! directed )
        insertEdge(j, i, w, G, 1);
}

void addWeight(int i, int j, float dW, graph *G, int directed) {
    edgenode *head = *(G->edges + i);
    while (head != NULL){
        if (head->y == j){
            head->w += dW;
            if ( ! directed )  // update weight point the other way
                addWeight(j, i, dW, G, 1);
            return;
        }
        head = head->nextedge;
    }
    insertEdge(i, j, dW, G, directed);
}

float deltaE(int i, int *s, graph *G) {
    float dE = 0.;
    edgenode *head = *(G->edges + i);
    while (head != NULL) {
        dE += s[head->y] * head->w;
        head = head->nextedge;
    }
    return 2 * s[i] * dE;
}

float deltaEsquarelattice(int i, int *s, float *J, int L) {
    int y = i / L, x = i % L;
    float *Jv = J + L*L;  // vertical bonds
    
    // right, down, left, up
    float dE = s[idx(y, MOD(x+1,L), L)] * J[i];
    dE += s[idx(MOD(y+1,L), x, L)] * Jv[i];
    dE += s[idx(y, MOD(x-1,L), L)] * J[idx(y, MOD(x-1,L), L)]; 
    dE += s[idx(MOD(y-1,L), x, L)] * Jv[idx(MOD(y-1,L), x, L)]; 
    return 2 * s[i] * dE;
}
