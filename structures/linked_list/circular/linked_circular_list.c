#include "linked_circular_list.h"

// n = 0
linkNode circular_insert_first(linkNode *manager)
{
    // allocate new node
    linkNode new = malloc(sizeof(struct linkedType));
    // set node next variable
    new->next = new;

    // update the manager
    *manager = new;
    // return the new node address
    return new;
}
// n > 0
linkNode circular_insert_after(linkNode node)
{
    // allocate new node
    linkNode new = malloc(sizeof(struct linkedType));
    // set node next variable
    new->next = node->next;

    node->next = new;

    return new;
}
// n > 0
linkNode circular_insert_end(linkNode *manager)
{
    *manager = circular_insert_after(*manager);
    return *manager;
}
// n > 1
void *circular_remove_end(linkNode *manager)
{
    linkNode before = (*manager)->next;

    while (before->next != *manager)
        before = before->next;

    *manager = before;

    return circular_remove_after(*manager);
}
// n > 1
void *circular_remove_after(linkNode node)
{
    linkNode old = node->next;
    node->next = old->next;
    void *data = old->data;
    free(old);
    return data;
}
// n = 1
void *circular_remove_last(linkNode *manager)
{
    void *data = (*manager)->data;
    free(*manager);

    *manager = NULL;
    return data;
}