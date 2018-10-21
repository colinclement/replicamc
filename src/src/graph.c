typedef struct {
    int Nv;
    edgenode **edges;
} graph;

typedef struct {
    int y;
    float w;
    edgenode *nextedge=NULL;
} edgenode;

graph* initGraph(int Nv)
{
    graph newGraph;
    newGraph.Nv = Nv;
    newGraph.edges = (edgenode **)malloc(Nv * sizeof(edgenode);
    for (int i=0; i < Nv; i++)
        *(newGraph.edges + i) = NULL;
    return newGraph;
}

void destroyGraph(graph *G)
{
    edgenode *tmp, *head;
    for (int i=0; i < G->Nv; i++)
    {
        head = G->edges + i;
        while (head != NULL)
        {
            tmp = head;
            head = head.nextedge;
            free(tmp);
        }
    }
    free(G);
}

void insertEdge(int i, int j, float w, graph *G)
{
    edgenode *oldEdge = G->edges + i;
    edgenode newEdge = {.y = j, .w = w, .nextedge = oldEdge};
    oldEdge = &newEdge;
    return;
}

float deltaE(int i, int *s, edgenode *G)
{
    int si = *(s+i), sj;
    float dE = 0.;
    edgenode *head=G->edges + i;
    while (head != NULL)
    {
        dE += si * (*(s+head.y)) * head.w;
        head = head.nextedge;
    }
    return 2*dE;
}
