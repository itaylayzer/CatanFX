#ifndef _INC_GRAPH

#define _INC_GRAPH

#include <stdlib.h>
#include <stdbool.h>

#include "../../controller/types.h"
#include "../avl_tree/avl_tree.h"
#include "../heap/heap.h"
#include "../queue/queue.h"
#include "../../model/headers/macro.h"

//----------------------------------------------------------------------------------------
//                                 graph init
//                                 ----------
//
// General      : The function initialize a graph
//
// Parameters   :
//          GraphPtr*  - Pointer to A Graph Pointer (In/Out)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void graph_init(GraphPtr *);

//----------------------------------------------------------------------------------------
//                                 graph join
//                                 ----------
//
// General      : The functions joins an edge between 2 vertecies.
//
// Parameters   :
//          graph      - Graph Pointer (In/Out)
//          from       - from vertex id (In)
//          to         - to vertex id (In)
//          color      - color of the edge (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void graph_join(GraphPtr graph,
                const unsigned char from,
                const unsigned char to,
                const unsigned char color);

///////////////////////////////////// DIJKSTRA ///////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 graph_dijkstra
//                                 --------------
//
// General      : The functions runs the dijkstra algorithm and affects the vertecies.
//
// Parameters   :
//          graph      - Graph Pointer (In/Out)
//          source     - source vertex id (In)
//          color      - edge allowed color (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void graph_dijkstra(GraphPtr graph, unsigned char source, unsigned char color);

//----------------------------------------------------------------------------------------
//                                 graph dijkstra distance
//                                 -----------------------
//
// General      : The functions calculate a distance between destiny vertext
//                to source vertex
//
// Parameters   :
//          graph      - Graph Pointer (In)
//          dest       - destiny vertex id (In)
//
// Return Value : Distance.
//
//----------------------------------------------------------------------------------------
unsigned char graph_dijkstra_distance(GraphPtr graph, unsigned char dest);

/////////////////////////////////// DFS ALGORITHM ////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 dfs score
//                                 ---------
//
// General      : The functions calculate the longest path of specific color.
//
// Parameters   :
//          graph      - Graph Pointer (In)
//          color      - allowed color (In)
//
// Return Value : Longest Path length.
//
//----------------------------------------------------------------------------------------
unsigned char dfs_score(GraphPtr graph, unsigned char color);
#endif