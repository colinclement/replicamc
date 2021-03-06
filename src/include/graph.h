#ifndef GRAPH_H
#define GRAPH_H

// how we can define the square lattice
#define MOD(x, N) (((x < 0) ? ((x % N) + N) : x) % N)

#define idx(y, x, L) (y*L + x)

#define neighbors(y, x, L) {idx(y, MOD(x-1, L), L),\
                            idx(y, MOD(x+1, L), L),\
                            idx(MOD(y-1,L), x, L),\
                            idx(MOD(y+1,L), x, L)}

#define uniqueneigh(y, x, L) {idx(y, MOD(x+1, L), L),\
                              idx(MOD(y+1,L), x, L)}

#define uniqueneighbonds(y, x, L) {idx(y, x, L),\
                                   L*L + idx(y, x, L)}

#define coordination 4

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
// directed = 0 makes a symmetric connection j->i as well
void insertEdge(int i, int j, float w, graph *G, int directed);

// if bond between i and j exists, update weight to w+dW,
// if bond does not exist create it and set weight to dW
void addWeight(int i, int j, float dW, graph *G, int directed);

// Compute the change in energy from flipping s[i] in graph G
float deltaE(int i, int *s, graph *G);

// Compute the change in energy from flipping s[i] in a square
// lattice of size LxL with bonds J
float deltaEsquarelattice(int i, int *s, float *J, int L);

#endif
