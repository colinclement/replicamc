#include <stdlib.h>
#include <stdio.h>
#include <graph.h>

int main(int argc, const char* argv[])
{
    graph *G = initGraph(3);    
    insertEdge(0, 1, 1, G);
    insertEdge(1, 2, 1, G);
    insertEdge(2, 1, 1, G);
    printGraph(G);
    int s[3] = {1, -1, -1};
    printf("%f\n", deltaE(0, s, G));

    destroyGraph(G);
    return 0;
}
