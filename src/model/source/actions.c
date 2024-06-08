#include "../headers/actions.h"

signed char compare_edges_offset(const void *first, const void *second)
{
    return ((EdgePtr)first)->offset - ((EdgePtr)second)->offset;
}
signed char value_compare(const void *first, const void *second)
{
    return first - second;
}

bool buy_road(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              bool transferMats,
              unsigned char from,
              unsigned char to)
{
    unsigned short edge_num;
    edge_num = from + (to << 8);

    avl_insert(&player->roads, convert_unsigned_short_to_void_ptr(edge_num), value_compare);

    player->amounts[ROAD]--;

    if (transferMats)
        transfer_materials(player, bank, cost, false);

    return change_road(player->color, graph, from, to);
}

bool change_road(unsigned char color,
                 GraphPtr graph,
                 unsigned char from,
                 unsigned char to)
{

    EdgePtr foundPtr;
    EdgeRec lookRec;

    lookRec.offset = to;

    foundPtr = avl_search(graph->vertices[from].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = color;

    lookRec.offset = from;
    foundPtr = avl_search(graph->vertices[to].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = color;

    return true;
}

void transfer_materials(PlayerPtr player,
                        signed char bank[TOTAL_MATERIALS],
                        const signed char cost[TOTAL_MATERIALS],
                        bool to_player)
{
    signed char *player_mats = (signed char *)player->materials;

    // remove cost from player materials
    signed char *new_materials = (to_player ? vector_add : vector_sub)(player_mats, cost, TOTAL_MATERIALS);
    vector_cpy(player_mats, (signed char *)new_materials, TOTAL_MATERIALS);
    free(new_materials);

    // update bank
    new_materials = (to_player ? vector_sub : vector_add)(bank, cost, TOTAL_MATERIALS);
    vector_cpy(bank, (signed char *)new_materials, TOTAL_MATERIALS);
    free(new_materials);
}

bool buy_city(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              unsigned char index)
{

    transfer_materials(player, bank, cost, false);

    // change settlement color
    graph->vertices[index].color = player->color | 0x40;

    player->victoryPoints++;
    player->amounts[CITY]--;

    return true;
}

unsigned char random_index_by_vals(unsigned char size, signed char *arr)
{
    signed char offset = -1, rand = brand(0, vector_sum(arr, size));

    while (rand > 0)
    {
        offset++;
        rand -= arr[offset];
    }
    return offset;
}

void transfer_dev_card(PlayerPtr player,
                       signed char bank[TOTAL_DEVELOPMENT_CARD],
                       const signed char *cost,
                       bool to_player)
{
    signed char *player_devs = (signed char *)player->developmentCards;

    // remove cost from player materials
    signed char *new_materials =
        (to_player ? vector_add : vector_sub)(player_devs, cost, TOTAL_DEVELOPMENT_CARD);
    vector_cpy(player_devs, new_materials, TOTAL_DEVELOPMENT_CARD);
    free(new_materials);

    // update bank
    new_materials =
        (to_player ? vector_sub : vector_add)(bank, cost, TOTAL_DEVELOPMENT_CARD);
    vector_cpy(bank, new_materials, TOTAL_DEVELOPMENT_CARD);
    free(new_materials);
}
bool buy_developement(PlayerPtr player,
                      signed char bank[TOTAL_MATERIALS],
                      signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                      const signed char cost[TOTAL_MATERIALS])
{

    signed char *how_many = calloc(TOTAL_DEVELOPMENT_CARD, sizeof(signed char));
    how_many[random_index_by_vals(TOTAL_DEVELOPMENT_CARD, dev_bank)] = 1;
    transfer_materials(player, bank, cost, false);
    transfer_dev_card(player, dev_bank, how_many, true);

    return true;
}