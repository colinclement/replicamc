#ifndef GRAPH_H
#define GRAPH_H

typedef struct edgenode edgenode;

// structure which points between vertices of a graph
struct edgenode {
    int y;
    float w;
    struct edgenode *nextedge;
};

// graph structure containing Nv vertives and an array of edge linked lists
typedef struct {
    int Nv;
    edgenode **edges;
} graph;

// Initialize a graph with Nv vertices
graph* initGraph(int Nv);

// Destroy graph G
void destroyGraph(graph *G);

// Print the connections of graph G
void printGraph(graph *G);

// Insert edge connecting i and j with weight w in graph G
void insertEdge(int i, int j, float w, graph *G);

// Compute the change in energy from flipping s[i] in graph G
float deltaE(int i, int *s, graph *G);

// Compute the change in energy from flipping s[i] in a square
// lattice of size LxL with bonds J
float deltaEsquare(int i, int L, int *s, float *J);

#endif
