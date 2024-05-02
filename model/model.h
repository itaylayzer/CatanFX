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
                         char *bank);
unsigned char *inf_player_actionable(unsigned char *size,
                                     PlayerPtr player,
                                     char dev_bank[TOTAL_DEVELOPMENT_CARD],
                                     const char store[TOTAL_STORE][TOTAL_MATERIALS]);
unsigned char *inf_player_materials(unsigned char *size,
                                    PlayerPtr players,
                                    unsigned char offset);
unsigned char *inf_player_devcards(unsigned char *size,
                                   PlayerPtr players,
                                   unsigned char offset);
unsigned char *switch_action_store(unsigned char *size,
                                   unsigned char *params,
                                   GraphPtr graph,
                                   PlayerPtr player,
                                   char mat_bank[TOTAL_MATERIALS],
                                   char devcard_bank[TOTAL_DEVELOPMENT_CARD],
                                   const char store[TOTAL_STORE][TOTAL_MATERIALS]);
unsigned char *single_byte(unsigned char *size, char value);
void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       char *bank);
//    bots
void bot_inits(PlayerPtr players, unsigned char num_of_players);
void handle_rest_turns(int socket,
                       unsigned char *turnOffset,
                       PlayerPtr players,
                       const unsigned num_of_players);
// mathematical
char *vector_join(const char *first,
                  const char *second,
                  signed char size,
                  char (*func)(char first, char second));
char *vector_map(const char *, signed char size, char (*func)(char));
char *vector_add(const char *first, const char *second, char size);
char *vector_sub(const char *first, const char *second, char size);
char *vector_neg(const char *, char size);
bool vector_any(const char *, signed char size, bool (*condition)(char));
bool vector_all(const char *, signed char size, bool (*condition)(char));
void vector_cpy(char *dest, char *src, signed char size);
void vector_val(char *dest, signed char size, char val);
char vec_sum(char *, signed char size);
char *vec_dup(char *, signed char size);
void vec_shuffle(char *, unsigned char size);
#endif