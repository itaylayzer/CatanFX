#define QUEUE_TRAVARSE_BODY_CONDITION(queue, node, cond) \
    while (!queue_empty((queue)) && cond)                \
    {                                                    \
        (node) = dequeue(&(queue));                      \
                                                         \
        ((node)->left != NULL) &&                        \
            enqueue(&(queue), (node)->left);             \
                                                         \
        ((node)->right != NULL) &&                       \
            enqueue(&(queue), (node)->right);

#define QUEUE_TRAVARSE_BODY(queue, node) \
    QUEUE_TRAVARSE_BODY_CONDITION(queue, node, true);

#define QUEUE_TRAVARSE_CONDITION(start, node, cond) \
    Queue queue;                                    \
    queue_init(&queue);                             \
    enqueue(&queue, (start));                       \
    QUEUE_TRAVARSE_BODY_CONDITION(queue, node, cond);

#define QUEUE_TRAVARSE(start, node) \
    QUEUE_TRAVARSE_CONDITION(start, node, true)

#define QUEUE_TRAVARSE_FINISH }

#define BOT_SEND(socket, size, buffer) \
    send(socket, &size, 1, 0);         \
    send(socket, buffer, size, 0);

#define BOT_SEND_FREE(socket, size, buffer) \
    BOT_SEND(socket, size, buffer);         \
    free(buffer);
