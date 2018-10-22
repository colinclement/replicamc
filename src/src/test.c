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
    pushint(1, &list);
    pushint(2141, &list);
    pushint(4, &list);
    printll(list);
    printf("is 0 in the list? %i\n", search(0, list));
    printf("is 1 in the list? %i\n", search(1, list));
    printf("is 5 in the list? %i\n", search(5, list));
    printf("is 2141 in the list? %i\n", search(2141, list));
    ll *list2 = initList(102);

    pushint(1230, &list2);

    lol *lolists = initListofLists(list);
    pushList(list2, &lolists);
    printlol(lolists);

    ll *list3 = initList(102);
    pushint(1230, &list3);
    pushint(10, &list3);
    //int popped = popint(&list3);
    //printf("popped %i\n", popped);
    printf("printing list3\n");
    printll(list3);
    //popint(&(list3->next->next));
    removeval(10, &list3);
    printf("printing list3\n");
    printll(list3);

    destroyListofLists(lolists);
    destroyList(list3);
    return 0;
}
