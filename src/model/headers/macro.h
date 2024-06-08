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

#define BOT_SEND(socket, size, buffer) \
    send(socket, &size, 1, 0);         \
    send(socket, buffer, size, 0);
#define BOT_SEND_FREE(socket, size, buffer) \
    BOT_SEND(socket, size, buffer);         \
    free(buffer);

#define endln puts("")
