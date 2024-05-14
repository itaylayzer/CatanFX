#ifndef _INC_TREE_TYPE

#define _INC_TREE_TYPE

typedef struct nodeType
{
    void *data;
    struct nodeType *left;
    struct nodeType *right;
    signed char height;
} NodeRec, *Node;

#endif