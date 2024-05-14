#include "queue.h"

void queue_init(QueuePtr manager)
{
    circular_init(manager);
}

void enqueue(QueuePtr manager, void *data)
{
    circular_insert_end(manager)->data = data;
}

bool queue_empty(Queue manager)
{
    return manager == NULL;
}

void *dequeue(QueuePtr manager)
{
    return circular_remove_after(manager);
}