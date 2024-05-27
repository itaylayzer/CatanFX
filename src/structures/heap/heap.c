#include "heap.h"

void swap(Heap *heap, unsigned char x, unsigned char y)
{
    heapInfo temp = heap->data[x];
    heap->data[x] = heap->data[y];
    heap->data[y] = temp;
}

void heap_init(Heap *heap, unsigned char size)
{
    heap->size = size;
    heap->length = 0;
    heap->data = malloc(sizeof(heapInfo) * size);
}

bool validate_index(Heap *heap, unsigned char index)
{
    return index < heap->length;
}

void heapify_up(Heap *heap, unsigned char index, signed char (*cmp)(signed char, signed char))
{
    unsigned char father = (index - 1) / 2;
    if (index > 0 && cmp(heap->data[index].score, heap->data[father].score) > 0)
    {
        swap(heap, index, father);
        heapify_up(heap, father, cmp);
    }
}

void heapify_down(Heap *heap, unsigned char index,
                  signed char (*cmp)(signed char, signed char))
{
    unsigned char leftChild = 2 * index + 1;
    unsigned char rightChild = 2 * index + 2;
    unsigned char largest = index;

    if (validate_index(heap, leftChild) && cmp(heap->data[leftChild].score, heap->data[largest].score) > 0)
    {
        largest = leftChild;
    }

    if (validate_index(heap, rightChild) && cmp(heap->data[rightChild].score, heap->data[largest].score) > 0)
    {
        largest = rightChild;
    }

    if (largest != index)
    {
        swap(heap, index, largest);
        heapify_down(heap, largest, cmp);
    }
}

void heap_insert(Heap *heap, void *data, signed char score,
                 signed char (*cmp)(signed char, signed char))
{
    heapInfo info;
    info.data = data;
    info.score = score;

    heap->data[heap->length] = info;
    heapify_up(heap, heap->length++, cmp);
}

void *heap_extract(Heap *heap, signed char (*cmp)(signed char, signed char))
{
    void *data = heap->data[0].data;
    heap->data[0].data = NULL;
    swap(heap, 0, --heap->length);
    if (heap->length)
        heapify_down(heap, 0, cmp);
    return data;
}

void *heap_top(Heap *heap)
{
    return heap->data[0].data;
}

signed char heap_min(signed char x, signed char y)
{
    return y - x;
}
signed char heap_max(signed char x, signed char y)
{
    return x - y;
}
bool heap_empty(Heap heap)
{
    return heap.length == 0;
}