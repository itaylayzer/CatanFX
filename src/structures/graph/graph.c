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
    edge->vertex = graph->vertices + to;
    edge->offset = to;

    avl_insert(&graph->vertices[from].edges, (void *)edge, edge_compare);
}

void dijkstra_relaxation(VertexPtr source, VertexPtr dest, unsigned char weight)
{
    if (dest->weight > source->weight + weight)
    {
        dest->weight = weight + source->weight;
        dest->father = source;
    }
}

void graph_dijkstra(GraphPtr graph, unsigned char source, unsigned char color)
{
    unsigned char offset;
    VertexPtr vertex;
    Heap scores;

    // dijkstra reset
    for (offset = 0; offset < VERTECIES; offset++)
    {
        graph->vertices[offset + AREAS].visited = false;
        graph->vertices[offset + AREAS].father = NULL;
        graph->vertices[offset + AREAS].weight = MAX_VALUE;
    }

    // set source
    vertex = graph->vertices + source;
    vertex->weight = 0;
    vertex->father = vertex;

    heap_init(&scores, VERTECIES);

    while (!heap_empty(scores))
    {
        vertex = heap_extract(&scores, heap_min, NULL);

        Queue queue;
        Node node;
        EdgePtr edge;
        VertexPtr dest;
        unsigned char vertex_weight;
        queue_init(&queue);
        enqueue(&queue, vertex->edges);

        // For each edge
        while (!queue_empty(queue) && vertex->visited == false)
        {
            node = dequeue(&queue);
            if (node->left != NULL)
            {
                enqueue(&queue, node->left);
            }

            if (node->right != NULL)
            {
                enqueue(&queue, node->right);
            }

            edge = node->data;
            dest = edge->vertex;
            if (edge->color == color)
            {
                vertex_weight = dest->weight;
                dijkstra_relaxation(vertex, dest, 1);
                if (vertex_weight > dest->weight)
                {
                    heap_insert(&scores, dest, dest->weight, heap_min);
                }
            }
        }

        while (!queue_empty(queue))
        {
            dequeue(&queue);
        }
    }
}

unsigned char graph_dijkstra_distance(GraphPtr graph, unsigned char dest)
{
    unsigned char distance = 0;
    VertexPtr vertex = graph->vertices + dest;
    while (vertex->father != vertex)
    {
        vertex = vertex->father;
        distance++;
    }
    return distance;
}