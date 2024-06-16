#include "graph.h"
#include <stdio.h>
void graph_init(GraphPtr *graph)
{
    // clear allocate the graph
    GraphPtr temp = calloc(1, sizeof(graph));

    // clear allocate the vertecies
    temp->vertices = calloc(AREAS + VERTECIES, sizeof(VertexRec));

    *graph = temp;
}

signed char edge_compare(const void *first, const void *second)
{
    // compare between the edge vertecies offsets
    return ((EdgePtr)first)->offset - ((EdgePtr)second)->offset;
}

void graph_join(GraphPtr graph,
                const unsigned char from,
                const unsigned char to,
                const unsigned char color)
{
    // allocate the edge
    EdgePtr edge = malloc(sizeof(EdgeRec));

    // define the edge variables
    edge->color = color;
    edge->vertex = graph->vertices + to;
    edge->offset = to;

    // avl insert to the vertecies[from].edges
    avl_insert(&graph->vertices[from].edges, (void *)edge, edge_compare);
}

void dijkstra_relaxation(VertexPtr source, VertexPtr dest, unsigned char weight)
{
    // relaxation
    (dest->weight > source->weight + weight) &&
        (dest->weight = weight + source->weight,
         dest->father = source,
         1);
}

void dijkstra_travarse(Heap *scores, unsigned char color)
{
    VertexPtr vertex;
    bool condition;
    Node node;
    EdgePtr edge;
    VertexPtr dest;
    unsigned char vertex_weight;

    // extract the top
    vertex = heap_extract(scores, heap_min, NULL);

    // travarse his adjacents
    QUEUE_TRAVARSE_CONDITION(vertex->edges, node, vertex->visited == false);
    edge = node->data;
    dest = edge->vertex;

    // if the color is the allowed color
    condition = (edge->color == color);

    // save the old weight
    vertex_weight = dest->weight;

    // apply relaxation
    (condition) &&
        (dijkstra_relaxation(vertex, dest, 1), 1);

    // insert to the heap if the new weight is below the old weight
    (condition && vertex_weight > dest->weight) &&
        heap_insert(scores, dest, dest->weight, heap_min);

    QUEUE_TRAVARSE_FINISH;

    // destroy the queue
    queue_destroy(&queue);
}

void graph_dijkstra(GraphPtr graph, unsigned char source, unsigned char color)
{
    unsigned char offset;
    Heap scores;
    VertexPtr vertex;

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

    // initialize heap
    heap_init(&scores, VERTECIES);

    // while heap isnt empty
    while (!heap_empty(scores))
    {
        dijkstra_travarse(&scores, color);
    }
}

unsigned char graph_dijkstra_distance(GraphPtr graph, unsigned char dest)
{
    unsigned char distance = 0;
    VertexPtr vertex = graph->vertices + dest;

    // while vertex is not null and have a father
    while (vertex && vertex->father != vertex)
    {
        // go back
        vertex = vertex->father;

        // increase distance length
        distance++;
    }

    // if the vertex is null negate the distance;
    distance = distance * ((vertex != NULL) * 2 - 1);

    return distance;
}

// O(E+V)
signed char dfs(GraphPtr graph, unsigned char vertex_offset, unsigned char targetColor, bool *visited)
{

    VertexPtr vertex = graph->vertices + vertex_offset;

    // stop condition: if the vertex is null or the color is gray or is visited
    if (vertex == NULL || vertex->color == GRAY || (visited && visited[vertex_offset - AREAS]))
    {
        return 0;
    }

    // if visited != NULL do visited[vertex] = true
    (visited) &&
        (visited[vertex_offset - AREAS] = true);

    signed char max_child_length = 0;

    Node node = vertex->edges;

    EdgePtr edge;

    // travarse edges
    QUEUE_TRAVARSE(node, node);
    edge = node->data;

    // if the edge color is the allowed color apply dfs again
    (edge->color == targetColor) &&
        (max_child_length = bmax(max_child_length, dfs(graph, edge->offset, targetColor, visited)));

    QUEUE_TRAVARSE_FINISH;

    return max_child_length + 1;
}

unsigned char dfs_score(GraphPtr graph, unsigned char color)
{
    unsigned char max_road = 0, vertex_offset;

    // for each vertex
    for (vertex_offset = 0; vertex_offset < VERTECIES; vertex_offset++)
    {

        // allocate visited booleans array
        bool *visited = calloc(VERTECIES, sizeof(bool));

        // apply the dfs and sets the max road number
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