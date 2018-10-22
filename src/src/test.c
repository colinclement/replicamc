#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main(int argc, const char* argv[])
{
    graph *G = initGraph(3);    
    insertEdge(0, 1, 0.1, G);
    insertEdge(1, 2, 0.5, G);
    insertEdge(2, 1, 3.21, G);
    printGraph(G);
    destroyGraph(G);
    return 0;
}
