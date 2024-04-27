#include "queue.h"

void queue_init(QueuePtr manager)
{
    circular_insert_first(manager);
}

void enqueue(QueuePtr manager, void *data)
{
    circular_insert_end(manager)->data = data;
}

bool queue_empty(QueuePtr manager)
{
    return (*manager)->next = *manager;
}

void *dequeue(QueuePtr manager)
{
    return circular_remove_end(manager);
}