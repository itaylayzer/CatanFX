#ifndef _INC_MODEL

#define _INC_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
// mathematical
char *vector_join(const char *first, const char *second, char size,
                  char (*func)(char first, char second));
char *vector_map(const char *, char size, char (*func)(char));
char *vector_add(const char *first, const char *second, char size);
char *vector_sub(const char *first, const char *second, char size);
char *vector_neg(const char *, char size);
bool vector_any(const char *, char size, bool (*condition)(char));
bool vector_all(const char *, char size, bool (*condition)(char));
void vector_cpy(char *dest, char *src, char size);
void vector_val(char *dest, char size, char val);
char vec_sum(char *, char size);
char *vec_dup(char *, char size);
void vec_shuffle(char *, unsigned char size);
#endif