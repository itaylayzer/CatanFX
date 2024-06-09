#include "queue.h"

void queue_init(QueuePtr manager)
{
    circular_init(manager);
}

bool enqueue(QueuePtr manager, void *data)
{
    linkNode node = circular_insert_end(manager);
    node->data = data;
    return !!node;
}

bool queue_empty(Queue manager)
{
    return manager == NULL;
}

void *dequeue(QueuePtr manager)
{
    return circular_remove_after(manager);
}

void destroy_queue(QueuePtr manager)
{
    while (!queue_empty(*manager))
    {
        dequeue(manager);
    }
}