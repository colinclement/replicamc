#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <ll.h>

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

    ll *list = initList(0);
    push(1, &list);
    push(2141, &list);
    push(4, &list);
    printll(list);
    printf("is 0 in the list? %i\n", search(0, list));
    printf("is 1 in the list? %i\n", search(1, list));
    printf("is 5 in the list? %i\n", search(5, list));
    printf("is 2141 in the list? %i\n", search(2141, list));

    destroyList(list);
    return 0;
}
