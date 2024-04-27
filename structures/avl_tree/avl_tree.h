#ifndef _INC_TREE

#define _INC_TREE

#include "../../types.h"
#include "stdlib.h"

#include "../../utils/math/compare.h"

Node make_tree(Node *);
Node avl_insert(Node *, void *element, char (*compare)(const void *, const void *));

#endif