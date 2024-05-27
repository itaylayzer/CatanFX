#ifndef _INC_HEAP

#define _INC_HEAP

#include <stdlib.h>
#include <stdbool.h>

typedef struct heapInfo
{
    void *data;
    signed char score;
} heapInfo;

typedef struct heapType
{
    struct heapInfo *data;
    unsigned char size;
    unsigned char length;
} Heap;

void heap_init(Heap *, unsigned char);

void heap_insert(Heap *, void *data, signed char score,
                 signed char (*)(signed char, signed char));

void *heap_extract(Heap *, signed char (*)(signed char, signed char));

void *heap_top(Heap *);

bool heap_empty(Heap);
signed char heap_min(signed char, signed char);
signed char heap_max(signed char, signed char);
#endif