#ifndef _INC_TREE

#define _INC_TREE

#include "stdlib.h"
#include <stdio.h>

#include "../../utils/math/compare.h"
#include "typedef.h"

//----------------------------------------------------------------------------------------
//                                 avl tree insert
//                                 ---------------
//
// General      : The functions add a value to an avl tree.
//
// Parameters   :
//          Node*      - Avl Tree (In/Out)
//          element    - Value (In)
//          compare    - compare functions (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void avl_insert(Node *, void *element, signed char (*compare)(const void *, const void *));

//----------------------------------------------------------------------------------------
//                                 avl search
//                                 ----------
//
// General      : The functions search Node based on its value.
//
// Parameters   :
//          node       - Avl Tree (In)
//          element    - Value (In)
//          compare    - compare functions (In)
//
// Return Value : Node if the value found, otherwise null.
//
//----------------------------------------------------------------------------------------
Node avl_search(Node node, void *element, signed char (*compare)(const void *, const void *));
#endif