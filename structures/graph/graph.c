#include "graph.h"
#include <stdio.h>
void graph_init(GraphPtr *graph)
{
    GraphPtr temp = calloc(1, sizeof(graph));
    temp->vertices = calloc(AREAS + VERTECIES, sizeof(VertexRec));

    *graph = temp;
}

signed char edge_compare(const void *first, const void *second)
{
    return ((EdgePtr)first)->offset - ((EdgePtr)second)->offset;
}

void graph_join(GraphPtr graph,
                const unsigned char from,
                const unsigned char to,
                const unsigned char color)
{
    EdgePtr edge = malloc(sizeof(EdgeRec));

    edge->color = color;
    edge->vertex = graph->vertices + from;
    edge->offset = to;

    void *voidPtr = edge;
    avl_insert(&graph->vertices[from].edges, voidPtr, edge_compare);
}