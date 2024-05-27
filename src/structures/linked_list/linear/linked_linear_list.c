#include "linked_linear_list.h"

// n = 0
void linear_init(linkNode *list)
{
    *list = NULL;
}
// i = 0
linkNode linear_push(linkNode *list)
{
    // allocate new node
    linkNode temp = malloc(sizeof(linkedRec));

    // connect
    temp->next = *list;
    *list = temp;

    return temp;
}
// i > 0
linkNode linear_insert_after(linkNode node)
{
    // allocate new node
    linkNode temp = malloc(sizeof(linkedRec));

    // connect
    temp->next = node->next;
    node->next = temp;

    return node;
}
// n = 0
void *linear_pop(linkNode *list)
{
    linkNode temp = *list;
    void *val = temp->data;
    *list = temp->next;
    free(temp);
    return val;
}
// n > 0
void *linear_remove_after(linkNode node)
{
    linkNode temp = node->next;
    void *val = node->next->data;
    node->next = temp->next;
    free(temp);
    return val;
}