#ifndef _INC_QUEUE

#define _INC_QUEUE

#include <stdbool.h>

#include "../linked_list/circular/linked_circular_list.h"

typedef linkNode Queue, *QueuePtr;

void queue_init(QueuePtr);

void enqueue(QueuePtr, void *data);

bool queue_empty(Queue);

void *dequeue(QueuePtr);

void destroy_queue(QueuePtr);

#endif