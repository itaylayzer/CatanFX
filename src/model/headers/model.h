#ifndef _INC_MODEL

#define _INC_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "../../utils/printt/printt.h"
#include "../../controller/types.h"
#include "../../controller/state.h"
#include "../../controller/constants.h"
#include "../../structures/graph/graph.h"
#include "../../structures/queue/queue.h"
#include "../../utils/math/compare.h"
#include "../../server/sock.h"
#include "../../utils/math/rand.h"
#include "../../utils/convert/convert.h"
#include "./vec_utils.h"
#include "./macro.h"
#include "./player.h"
#include "./actions.h"

// initializations
void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2]);
void catan_players_init(PlayerPtr players, signed char size);
// information
unsigned char *areas_numbers(unsigned char *size, signed char *params,
                             int socket, GameState state);
unsigned char *harbors_numbers(unsigned char *size, signed char *params,
                               int socket, GameState state);
unsigned char *players_around_area(unsigned char *size, signed char *params,
                                   int socket, GameState state);
// actions
unsigned char *roll_dice(unsigned char *size, signed char *params,
                         int socket, GameState state);
unsigned char *inf_player_actionable(unsigned char *size, signed char *params,
                                     int socket, GameState state);
unsigned char *inf_player_materials(unsigned char *size, signed char *params,
                                    int socket, GameState state);
unsigned char *inf_player_devcards(unsigned char *size, signed char *params,
                                   int socket, GameState state);
unsigned char *inf_player_victory_points(unsigned char *size, signed char *params,
                                         int socket, GameState state);
unsigned char *inf_player_amounts(unsigned char *size, signed char *params,
                                  int socket, GameState state);
unsigned char *inf_achivement_cards(unsigned char *size, signed char *params,
                                    int socket, GameState state);
unsigned char *switch_action_store(unsigned char *size, signed char *params,
                                   int socket, GameState state);
unsigned char *switch_dev_card(unsigned char *size, signed char *params,
                               int socket, GameState state);
unsigned char *single_byte(unsigned char *size, signed char value);
void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank,
                       const unsigned char robberArea);
unsigned char *move_robber(unsigned char *size, signed char *params,
                           int socket, GameState state);

unsigned char *drop_materials(unsigned char *size, signed char *params,
                              int socket, GameState state);
unsigned char *make_a_deal(unsigned char *size, signed char *params, int socket,
                           GameState state);
//    bots
unsigned char *handle_rest_turns(unsigned char *size, signed char *params,
                                 int socket, GameState state);

#endif