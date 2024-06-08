#ifndef _INC_PLAYER

#define _INC_PLAYER

#include <stdlib.h>
#include <stdbool.h>

#include "../../utils/printt/printt.h"
#include "../../structures/stack/stack.h"
#include "../../structures/queue/queue.h"
#include "../../structures/heap/heap.h"
#include "../../controller/types.h"
#include "../../controller/constants.h"
#include "../../utils/convert/convert.h"
#include "./vec_utils.h"
#include "./macro.h"
#include "../../structures/graph/graph.h"
#include "./actions.h"

// helper
bool buyableSettlement(PlayerPtr player, VertexPtr source, bool needRoad);

// astrategies scores
float woodScore(float[TOTAL_MATERIALS]);
float wheatScore(float[TOTAL_MATERIALS]);
float cardsScore(float[TOTAL_MATERIALS]);

// picking astrategies
unsigned char *astrategies_init(GraphPtr graph, Heap heaps[TOTAL_ASTRATEGIES]);

// astrategies prioritise functionsת TODO:
unsigned short prioritiseWoodRoad(GraphPtr, PlayerPtr, Heap[TOTAL_ASTRATEGIES],
                                  StackPtr);
unsigned short prioritiseWheatCardsRoad(GraphPtr, PlayerPtr, Heap[TOTAL_ASTRATEGIES],
                                        StackPtr);
unsigned char prioritiseWoodSettlement(GraphPtr, PlayerPtr, Heap[TOTAL_ASTRATEGIES]);
unsigned char prioritiseWheatCardsSettlement(GraphPtr, PlayerPtr, Heap[TOTAL_ASTRATEGIES]);

unsigned char *woodMatsOrder(GraphPtr, PlayerPtr);
unsigned char *wheatMatsOrder(GraphPtr, PlayerPtr);
unsigned char *cardsMatsOrder(GraphPtr, PlayerPtr);

// astrategies play function TODO:
void wood_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                       void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr));
void wheat_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                        void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr));
void cards_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                        void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr));
void play_actions_with_conditions(GameState state,
                                  PlayerPtr player,
                                  int socket,
                                  bool (**conditions)(PlayerPtr, GameState, QueuePtr),
                                  void (**actions)(PlayerPtr, int, GameState, QueuePtr),
                                  unsigned char size);

bool state_can_buy_road(PlayerPtr, GameState, QueuePtr);
bool state_can_buy_settlement(PlayerPtr, GameState, QueuePtr);
bool state_can_buy_city(PlayerPtr, GameState, QueuePtr);
bool state_can_buy_development(PlayerPtr, GameState, QueuePtr);
void state_buy_road(PlayerPtr, int, GameState, QueuePtr);
void state_buy_settlement(PlayerPtr, int, GameState, QueuePtr);
void state_buy_city(PlayerPtr, int, GameState, QueuePtr);
void state_buy_development(PlayerPtr, int, GameState, QueuePtr);

// generic functions
unsigned char prioritiseUpgradeableSettlement(PlayerPtr player, GraphPtr graph,
                                              float (*)(float[TOTAL_MATERIALS]));
unsigned char moveRobberTo(PlayerPtr player, GraphPtr graph);
unsigned char buyableRoadsAroundVertex(GraphPtr graph, StackPtr stk, PlayerPtr player, unsigned char);
unsigned char buyableRoads(GraphPtr graph, StackPtr stk, PlayerPtr player);
unsigned char buyableSettlements(StackPtr, PlayerPtr player, GraphPtr graph, bool needRoad);
unsigned char upgradeableSettlements(StackPtr, PlayerPtr player, GraphPtr graph);
bool buyableProduct(unsigned char astrategy,
                    PlayerPtr player,
                    GraphPtr graph,
                    signed char playerMats[TOTAL_MATERIALS],
                    signed char playerHarbors,
                    const signed char productMats[TOTAL_MATERIALS],
                    QueuePtr actionsQ);

#endif