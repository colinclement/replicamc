#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <ll.h>

graph* initGraph(int Nv)
{
    graph *newGraph = (graph *)malloc(sizeof(graph));
    newGraph->Nv = Nv;
    newGraph->edges = (edgenode **)malloc(Nv * sizeof(edgenode*));
    if (newGraph->edges == NULL)
    {
        fprintf(stderr, "Failed to allocate new graph\n");
        free(newGraph->edges);
        exit(0);
    }
    
    for (int i=0; i < Nv; i++)
        *(newGraph->edges + i) = NULL;
    return newGraph;
}

void destroyGraph(graph *G)
{
    edgenode *tmp, *head;
    for (int i=0; i < G->Nv; i++)
    {
        head = *(G->edges + i);
        while (head != NULL)
        {
            tmp = head;
            head = head->nextedge;
            free(tmp);
        }
    }
    free(G->edges);
    free(G);
}

void printGraph(graph *G)
{
    printf("Graph with %i vertices\n", G->Nv);
    edgenode *head;
    for (int i=0; i < G->Nv; i++)
    {
        head = *(G->edges + i);
        while (head != NULL)
        {
            printf("%i connected to %i with weight %f\n", i, head->y, head->w);
            head = head->nextedge;
        }
    }
}

void insertEdge(int i, int j, float w, graph *G)
{
    edgenode *newEdge = (edgenode *)malloc(sizeof(edgenode));
    if (newEdge == NULL)
    {
        fprintf(stderr, "Failed to allocate new edge\n");
        free(newEdge);
        exit(0);
    }
    newEdge->y = j;
    newEdge->w = w;
    newEdge->nextedge = *(G->edges+i);
    *(G->edges+i) = newEdge;
}

float deltaE(int i, int *s, graph *G)
{
    float dE = 0.;
    edgenode *head = *(G->edges + i);
    while (head != NULL)
    {
        dE += s[head->y] * head->w;
        head = head->nextedge;
    }
    return 2 * s[i] * dE;
}

#define idx(y, x, L) (y*L + x)

float deltaEsquarelattice(int i, int *s, float *J, int L)
{
    int y = i / L, x = i % L;
    float *Jv = J + L*L;  // vertical bonds
    
    // right, down, left, up
    float dE = s[idx(y, MOD(x+1,L), L)] * J[i];
    dE += s[idx(MOD(y+1,L), x, L)] * Jv[i];
    dE += s[idx(y, MOD(x-1,L), L)] * J[idx(y, MOD(x-1,L), L)]; 
    dE += s[idx(MOD(y-1,L), x, L)] * Jv[idx(MOD(y-1,L), x, L)]; 
    return 2 * s[i] * dE;
}
