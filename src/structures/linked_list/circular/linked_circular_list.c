#include "linked_circular_list.h"

// n = 0
void circular_init(linkNode *manager)
{
    *manager = NULL;
}
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
    linkNode new = malloc(sizeof(linkedRec));
    // set node next variable
    new->next = node->next;

    node->next = new;

    return new;
}
// n > 0
linkNode circular_insert_end(linkNode *manager)
{
    if (*manager == NULL)
    {
        return circular_insert_first(manager);
    }
    return (*manager = circular_insert_after(*manager));
}
// n > 1
void *circular_remove_end(linkNode *manager)
{
    linkNode before = (*manager)->next;

    while (before->next != *manager)
        before = before->next;

    *manager = before;

    return circular_remove_after(manager);
}

// n > 1
void *circular_remove_after(linkNode *head)
{
    linkNode old = (*head)->next;

    void *data = old->data;

    if (*head == (*head)->next)
    {
        free(*head);
        *head = NULL;
    }
    else
    {
        (*head)->next = old->next;
        free(old);
    }

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