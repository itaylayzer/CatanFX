#ifndef _INC_STATE

#define _INC_STATE

#include "./types.h"
#include "../structures/graph/graph.h"

typedef struct
{
    GraphPtr graph;
    unsigned char *harbors;
    PlayerPtr players;
    signed char *bankDevelopments;
    signed char *bankMaterials;
    unsigned char turnOffset;
    const unsigned char num_of_players;
    signed char *achievementCards;
    unsigned char robberArea;
    Heap *astHeaps;
    unsigned char *astIndexes;
} stateRec, *GameState;

#endif