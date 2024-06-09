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
    (dest->weight > source->weight + weight) &&
        (dest->weight = weight + source->weight,
         dest->father = source,
         1);
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

        Node node;
        EdgePtr edge;
        VertexPtr dest;
        unsigned char vertex_weight;

        QUEUE_TRAVARSE_CONDITION(vertex->edges, node, vertex->visited == false);
        edge = node->data;
        dest = edge->vertex;
        if (edge->color == color)
        {
            vertex_weight = dest->weight;
            dijkstra_relaxation(vertex, dest, 1);

            (vertex_weight > dest->weight) &&
                heap_insert(&scores, dest, dest->weight, heap_min);
        }

        QUEUE_TRAVARSE_FINISH;
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
    puts("graph_dijkstra_distance");
    while (vertex && vertex->father != vertex)
    {
        vertex = vertex->father;
        distance++;
    }
    return distance * ((vertex != NULL) * 2 - 1);
}

// O(E+V)
signed char dfs(GraphPtr graph, unsigned char vertex_offset, unsigned char targetColor, bool *visited)
{
    VertexPtr vertex = graph->vertices + vertex_offset;
    if (vertex == NULL || vertex->color == GRAY || (visited && visited[vertex_offset - AREAS]))
    {
        return 0;
    }

    (visited) &&
        (visited[vertex_offset - AREAS] = true);

    signed char max_child_length = 0;

    Node node = vertex->edges;

    EdgePtr edge;

    QUEUE_TRAVARSE(node, node);
    edge = node->data;

    (edge->color == targetColor) &&
        (max_child_length = bmax(max_child_length, dfs(graph, edge->offset, targetColor, visited)));

    QUEUE_TRAVARSE_FINISH;

    return max_child_length + 1;
}

unsigned char dfs_score(GraphPtr graph, unsigned char color)
{
    unsigned char max_road = 0, vertex_offset;

    for (vertex_offset = 0; vertex_offset < VERTECIES; vertex_offset++)
    {
        bool *visited = calloc(VERTECIES, sizeof(bool));
        max_road = bmax(max_road,
                        dfs(graph,
                            vertex_offset + AREAS,
                            color,
                            visited) -
                            1);
        free(visited);
    }

    return max_road;
}