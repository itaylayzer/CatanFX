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

#include "../utils/printt/printt.h"
#include "../types.h"
#include "../structures/graph/graph.h"
#include "../structures/queue/queue.h"
#include "../utils/math/compare.h"
#include "../server/sock.h"

// game logic
void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2]);
void catab_players_init(PlayerPtr players, signed char size);
unsigned char *areas_numbers(unsigned char *size, GraphPtr graph);
unsigned char *harbors_numbers(unsigned char *size,
                               GraphPtr graph,
                               unsigned char harbors[HARBOR_COUNT * 2]);
unsigned char *roll_dice(unsigned char *size,
                         PlayerPtr players,
                         GraphPtr graph,
                         signed char *bank);
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
                                   const signed char store[TOTAL_STORE][TOTAL_MATERIALS], signed char achievements[TOTAL_ACHIEVEMENTS_CARD]);
unsigned char *single_byte(unsigned char *size, signed char value);
void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank);
//    bots
void bot_inits(PlayerPtr players, unsigned char num_of_players);
void handle_rest_turns(int socket,
                       unsigned char *turnOffset,
                       PlayerPtr players,
                       const unsigned num_of_players);
// mathematical
signed char *vector_join(const signed char *first,
                         const signed char *second,
                         signed char size,
                         signed char (*func)(signed char first, signed char second));
signed char *vector_map(const signed char *, signed char size, signed char (*func)(signed char));
signed char *vector_add(const signed char *first, const signed char *second, signed char size);
signed char *vector_sub(const signed char *first, const signed char *second, signed char size);
signed char *vector_neg(const signed char *, signed char size);
bool vector_any(const signed char *, signed char size, bool (*condition)(signed char));
bool vector_all(const signed char *, signed char size, bool (*condition)(signed char));
void vector_cpy(signed char *dest, signed char *src, signed char size);
void vector_val(signed char *dest, signed char size, signed char val);
signed char vec_sum(signed char *, signed char size);
signed char *vec_dup(signed char *, signed char size);
void vec_shuffle(signed char *, unsigned char size);
#endif