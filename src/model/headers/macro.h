#define QUEUE_TRAVARSE_BODY(queue, node)      \
    while (!queue_empty((queue)))             \
    {                                         \
        (node) = dequeue(&(queue));           \
        if ((node)->left != NULL)             \
        {                                     \
            enqueue(&(queue), (node)->left);  \
        }                                     \
        if ((node)->right != NULL)            \
        {                                     \
            enqueue(&(queue), (node)->right); \
        }

#define QUEUE_TRAVARSE(start, node) \
    Queue queue;                    \
    queue_init(&queue);             \
    enqueue(&queue, (start));       \
    QUEUE_TRAVARSE_BODY(queue, node)

#define QUEUE_TRAVARSE_FINISH }
