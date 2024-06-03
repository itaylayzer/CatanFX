#ifndef _INC_MODEL

#define _INC_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "../../utils/printt/printt.h"
#include "../../controller/types.h"
#include "../../structures/graph/graph.h"
#include "../../structures/queue/queue.h"
#include "../../utils/math/compare.h"
#include "../../server/sock.h"
#include "../../utils/math/rand.h"
#include "../../utils/convert/convert.h"
#include "./vec_utils.h"
#include "./macro.h"

// initializations
void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2]);
void catan_players_init(PlayerPtr players, signed char size);
// information
unsigned char *areas_numbers(unsigned char *size, GraphPtr graph);
unsigned char *harbors_numbers(unsigned char *size,
                               GraphPtr graph,
                               unsigned char harbors[HARBOR_COUNT * 2]);
unsigned char *players_around_area(unsigned char *size,
                                   unsigned char vertex_area,
                                   GraphPtr graph);
// actions
unsigned char *roll_dice(unsigned char *size,
                         PlayerPtr players,
                         GraphPtr graph,
                         signed char *bank,
                         const unsigned char robberArea);
unsigned char *inf_player_actionable(unsigned char *size,
                                     PlayerPtr player,
                                     signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                                     const signed char store[TOTAL_STORE][TOTAL_MATERIALS]);
unsigned char *inf_player_materials(unsigned char *size,
                                    PlayerPtr players,
                                    signed char *bankMaterials,
                                    unsigned char offset);
unsigned char *inf_player_devcards(unsigned char *size,
                                   PlayerPtr players,
                                   signed char *bankDevelopment,
                                   unsigned char offset);
unsigned char *inf_player_amounts(unsigned char *size, PlayerPtr player);
unsigned char *switch_action_store(unsigned char *size,
                                   signed char *params,
                                   GraphPtr graph,
                                   PlayerPtr player,
                                   signed char mat_bank[TOTAL_MATERIALS],
                                   signed char devcard_bank[TOTAL_DEVELOPMENT_CARD],
                                   const signed char store[TOTAL_STORE][TOTAL_MATERIALS], signed char *longest_road_achievement);
unsigned char *switch_dev_card(unsigned char *size,
                               PlayerPtr players,
                               unsigned char num_of_players,
                               signed char *bankMaterials,
                               signed char *params);
unsigned char *single_byte(unsigned char *size, signed char value);
void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank,
                       const unsigned char robberArea);
unsigned char *move_robber(unsigned char *size,
                           PlayerPtr players,
                           const unsigned char player_index,
                           unsigned char *robberArea,
                           signed char *params,
                           signed char *biggest_army_achivement,
                           const unsigned char num_of_players);

unsigned char *drop_materials(unsigned char *size, PlayerPtr player, signed char *mats);
//    bots
void bot_inits(PlayerPtr players, unsigned char num_of_players);
void handle_rest_turns(int socket,
                       GraphPtr graph,
                       PlayerPtr players,
                       signed char *bankDevelopments,
                       signed char *bankMaterials,
                       unsigned char *turnOffset,
                       const unsigned char num_of_players,
                       signed char *achievementCards,
                       unsigned char *robberArea,
                       Heap astHeaps[TOTAL_ASTRATEGIES],
                       unsigned char *astIndexes);

#endif