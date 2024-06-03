#ifndef _INC_PLAYER

#include <stdlib.h>
#include <stdbool.h>

#include "../../structures/stack/stack.h"
#include "../../structures/queue/queue.h"
#include "../../structures/heap/heap.h"
#include "../../controller/types.h"
#include "../../controller/constants.h"
#include "../../utils/convert/convert.h"
#include "./vec_utils.h"
#include "./macro.h"

// astrategies scores
float woodScore(float[TOTAL_MATERIALS]);
float wheatScore(float[TOTAL_MATERIALS]);
float cardsStore(float[TOTAL_MATERIALS]);

// picking astrategies
unsigned char *astrategies_init(GraphPtr graph, Heap heaps[TOTAL_ASTRATEGIES]);

// astrategies prioritise functions
void prioritiseWoodRoad();
void prioritiseWheatCardsRoad();
void prioritiseWoodSettlement();
void prioritiseWheatCardsSettlement();
unsigned char *woodMatsOrder(PlayerPtr, GraphPtr);
unsigned char *wheatMatsOrder(PlayerPtr, GraphPtr);
unsigned char *cardsMatsOrder(PlayerPtr, GraphPtr);

// astrategies play function
// TODO:

// generic functions
unsigned char prioritiseUpgradeableSettlement(PlayerPtr player, GraphPtr graph,
                                              float (*)(float[TOTAL_MATERIALS]));
unsigned char moveRobberTo(PlayerPtr player, GraphPtr graph);
unsigned char buyableRoads(StackPtr, PlayerPtr player);
unsigned char buyableSettlements(StackPtr, PlayerPtr player, GraphPtr graph, bool needRoad);
unsigned char upgradeableSettlements(StackPtr, PlayerPtr player, GraphPtr graph);
bool buyableProduct(unsigned char astrategy,
                    PlayerPtr player,
                    GraphPtr graph,
                    signed char playerMats[TOTAL_MATERIALS],
                    signed char playerHarbors,
                    unsigned char productMats[TOTAL_MATERIALS],
                    QueuePtr actionsQ)
#endif