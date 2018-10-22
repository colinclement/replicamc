#ifndef GRAPH_H
#define GRAPH_H

typedef struct edgenode edgenode;

struct edgenode {
    int y;
    float w;
    struct edgenode *nextedge;
};

typedef struct {
    int Nv;
    edgenode **edges;
} graph;

graph* initGraph(int Nv);

void destroyGraph(graph *G);

void printGraph(graph *G);

void insertEdge(int i, int j, float w, graph *G);

float deltaE(int i, int *s, graph *G);

float deltaEsquare(int i, int L, int *s, float *J);

#endif
