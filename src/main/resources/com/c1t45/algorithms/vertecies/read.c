#include <stdio.h>
#include <stdlib.h>

void main()
{
    FILE *file;
    file = fopen("edges.txt", "rb");

    if (!file)
        exit(EXIT_FAILURE);

    char elements[2];
    while (!feof(file))
    {
        fread(elements, sizeof(char), 2, file);
        printf("%hhd %hhd\n", elements[0], elements[1]);
    }

    fclose(file);
}