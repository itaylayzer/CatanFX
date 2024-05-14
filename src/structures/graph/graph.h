#ifndef _INC_GRAPH

#define _INC_GRAPH

#include <stdlib.h>
#include <stdbool.h>

#include "../../types.h"
#include "../avl_tree/avl_tree.h"

void graph_init(GraphPtr *);
void graph_join(GraphPtr graph,
                const unsigned char from,
                const unsigned char to,
                const unsigned char color);
#endif