#ifndef _INC_ACTIONS

#define _INC_ACTIONS

#include "../../controller/controller.h"

signed char value_compare(const void *first, const void *second);

bool transfer_materials(PlayerPtr player,
                        signed char bank[TOTAL_MATERIALS],
                        const signed char cost[TOTAL_MATERIALS],
                        bool to_player);
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
bool buy_developement(PlayerPtr player,
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
#endif