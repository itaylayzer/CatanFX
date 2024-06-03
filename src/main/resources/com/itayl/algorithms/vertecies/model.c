#include "types.h"
#include <stdio.h>
#include <stdlib.h>

GraphRec graph;

void main(void)
{
    FILE *file;
    file = fopen("edges.txt", "rb");
     
    if (!file)
        exit(EXIT_FAILURE);
     
    char elements[2];
    while (!feof(file))
    {
        fread(elements, sizeof(char), 2, file);
        avl_insert(graph.vertices[elements[0]]->edges, );
        printf("%hhd %hhd\n", elements[0], elements[1]);
    }

    fclose(file);
}