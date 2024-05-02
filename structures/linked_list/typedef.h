#ifndef _INC_LINKED_LIST_TYPE

#define _INC_LINKED_LIST_TYPE

typedef struct linkedType
{
    void *data;
    struct linkedType *next;
} linkedRec, *linkNode;

#endif