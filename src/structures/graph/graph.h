#ifndef _INC_GRAPH

#define _INC_GRAPH

#include <stdlib.h>
#include <stdbool.h>

#include "../../controller/types.h"
#include "../avl_tree/avl_tree.h"
#include "../heap/heap.h"
#include "../queue/queue.h"
#include "../../model/headers/macro.h"

void graph_init(GraphPtr *);
void graph_join(GraphPtr graph,
                const unsigned char from,
                const unsigned char to,
                const unsigned char color);

// dijkstra algorithm
void graph_dijkstra(GraphPtr graph, unsigned char source, unsigned char color);
unsigned char graph_dijkstra_distance(GraphPtr graph, unsigned char dest);

// O(E+VE)
unsigned char dfs_score(GraphPtr graph, unsigned char color);
#endif