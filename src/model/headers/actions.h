#ifndef _INC_ACTIONS

#define _INC_ACTIONS

#include "../../controller/controller.h"

signed char value_compare(const void *first, const void *second);

bool transfer_materials(PlayerPtr player,
                        signed char bank[TOTAL_MATERIALS],
                        const signed char cost[TOTAL_MATERIALS],
                        bool to_player);
void transfer_all_players_mats(PlayerPtr players,
                               unsigned char player_index,
                               signed char num_of_players,
                               unsigned char mat);
// O(CE+CVE+C) | C is negligible
signed char update_longest_road(GraphPtr graph, signed char *longest_road_achievement);
signed char update_biggest_army(PlayerPtr players,
                                signed char num_of_players,
                                signed char *biggest_army_achievement);
bool buy_road(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              bool transferMats,
              unsigned char from,
              unsigned char to);

bool change_road(unsigned char color,
                 GraphPtr graph,
                 unsigned char from,
                 unsigned char to);
bool buy_city(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              unsigned char index);
void transfer_dev_card(PlayerPtr player,
                       signed char bank[TOTAL_DEVELOPMENT_CARD],
                       const signed char *cost,
                       bool to_player);
unsigned char buy_developement(PlayerPtr player,
                               signed char bank[TOTAL_MATERIALS],
                               signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                               const signed char cost[TOTAL_MATERIALS]);
unsigned char random_index_by_vals(unsigned char size, signed char *arr);
bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const signed char cost[TOTAL_MATERIALS],
                    signed char bank[TOTAL_MATERIALS],
                    signed char transferMats, // -1 to player 1 from player
                    unsigned char index);
unsigned char extract_area_materials(unsigned char material_number);

// use dev_card bot actions
void use_dev_knight(PlayerPtr, int, GameState);
void use_dev_point(PlayerPtr, int, GameState);
void use_dev_roads(PlayerPtr, int, GameState);
void use_dev_yop(PlayerPtr, int, GameState);
void use_dev_monopol(PlayerPtr, int, GameState);

// socket shortcuts
void socket_short_update(int);
void socket_short_log(int, unsigned char);

#endif