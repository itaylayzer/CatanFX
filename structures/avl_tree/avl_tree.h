#ifndef _INC_TREE

#define _INC_TREE
#include "stdlib.h"

#include "../../utils/math/compare.h"
#include "typedef.h"

Node make_tree(Node *);
Node avl_insert(Node *, void *element, signed char (*compare)(const void *, const void *));
Node avl_search(Node node, void *element, signed char (*compare)(const void *, const void *));
#endif