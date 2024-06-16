#include "heap.h"

void swap(Heap *heap, unsigned char x, unsigned char y)
{
    // swap heaps elements
    heapInfo temp = heap->data[x];
    heap->data[x] = heap->data[y];
    heap->data[y] = temp;
}

void heap_init(Heap *heap, unsigned char size)
{
    // reset the size and length
    heap->size = size;
    heap->length = 0;

    // allocate the heapInfo array
    heap->data = malloc(sizeof(heapInfo) * size);
}

bool validate_index(Heap *heap, unsigned char index)
{
    return index < heap->length;
}

void heapify_up(Heap *heap, unsigned char index, signed char (*cmp)(signed char, signed char))
{
    unsigned char father = (index - 1) / 2;

    // continue heapify up if index is valid and the fathers score
    //      is still above current score
    (index > 0 && cmp(heap->data[index].score, heap->data[father].score) > 0) &&
        (swap(heap, index, father),
         heapify_up(heap, father, cmp),
         1);
}

void heapify_down(Heap *heap, unsigned char index,
                  signed char (*cmp)(signed char, signed char))
{
    // calculate right and left children indexes
    unsigned char leftChild = 2 * index + 1;
    unsigned char rightChild = 2 * index + 2;
    unsigned char largest = index;

    // change largest to be left child if exists and it scores are above current element score
    (validate_index(heap, leftChild) && cmp(heap->data[leftChild].score, heap->data[largest].score) > 0) &&
        (largest = leftChild);

    // change largest to be right child if exists and it scores are above current element score or above left child score
    (validate_index(heap, rightChild) && cmp(heap->data[rightChild].score, heap->data[largest].score) > 0) &&
        (largest = rightChild);

    // if largest is one of the childs continue heapify down
    (largest != index) &&
        (swap(heap, index, largest),
         heapify_down(heap, largest, cmp),
         1);
}

bool heap_insert(Heap *heap, void *data, signed char score,
                 signed char (*cmp)(signed char, signed char))
{
    // change values
    heapInfo info;
    info.data = data;
    info.score = score;

    // put in the heap
    heap->data[heap->length] = info;

    // heapify_up and increase the length
    heapify_up(heap, heap->length++, cmp);

    return true;
}

void *heap_extract(Heap *heap,
                   signed char (*cmp)(signed char, signed char),
                   signed char *scoreptr)
{
    // extract data
    void *data = heap->data[0].data;
    signed char score = heap->data[0].score;

    // reset data
    heap->data[0].data = NULL;

    // swap data to the last index
    swap(heap, 0, --heap->length);

    // apply heapify down
    (heap->length) &&
        (heapify_down(heap, 0, cmp), 1);

    // if score ptr != null set the score
    (scoreptr) &&
        (*scoreptr = score);

    return data;
}

void *heap_top(Heap *heap)
{
    return heap->data[0].data;
}

signed char heap_top_score(Heap *heap)
{
    return heap->data[0].score;
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
void heap_destroy(Heap *heap)
{
    free(heap->data);
}