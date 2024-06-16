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
    putts("transfer_materials buy_road");

    transferMats &&transfer_materials(player, bank, cost, false);

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

bool transfer_materials(PlayerPtr player,
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

    if (vector_any((signed char *)player->materials, TOTAL_MATERIALS, below_zero))
    {
        puts("transfer_materials player->materials");
        print_vec(player->materials, TOTAL_MATERIALS);

        exit(0);
    }
    if (vector_any(bank, TOTAL_MATERIALS, below_zero))
    {
        puts("transfer_materials bank");
        print_vec((unsigned char *)bank, TOTAL_MATERIALS);
        exit(0);
    }

    return true;
}
void transfer_all_players_mats(PlayerPtr players,
                               unsigned char player_index,
                               signed char num_of_players,
                               unsigned char mat)
{
    PlayerPtr other;
    bool is_other_player;
    while (--num_of_players >= 0)
    {
        other = players + num_of_players;
        signed char mats_to_transfer[TOTAL_MATERIALS] = {0};

        is_other_player = (num_of_players != player_index);

        mats_to_transfer[mat] = is_other_player * other->materials[mat];
        putts("transfer_materials transfer_all_players_mats");
        is_other_player &&
            transfer_materials(players + player_index, (signed char *)other->materials, mats_to_transfer, true);
    }
}

bool buy_city(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              unsigned char index)
{
    putts("transfer_materials buy city");
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
unsigned char buy_developement(PlayerPtr player,
                               signed char bank[TOTAL_MATERIALS],
                               signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                               const signed char cost[TOTAL_MATERIALS])
{

    signed char *how_many = calloc(TOTAL_DEVELOPMENT_CARD, sizeof(signed char));
    unsigned char index = random_index_by_vals(TOTAL_DEVELOPMENT_CARD, dev_bank);
    how_many[index] = 1;
    putts("transfer_materials buy development");
    transfer_materials(player, bank, cost, false);
    transfer_dev_card(player, dev_bank, how_many, true);

    return index;
}
unsigned char extract_area_materials(unsigned char material_number)
{
    return material_number & 0x07;
}

unsigned char *svertex_to_materials(GraphPtr graph, signed char index)
{
    Queue que;
    EdgePtr edge;
    unsigned char material_number, size = 0, *mats = calloc(TOTAL_MATERIALS, sizeof(char));
    Node node = graph->vertices[index].edges;

    QUEUE_TRAVARSE(node, node);
    size++;
    edge = (EdgePtr)node->data;
    material_number = edge->vertex->material_number;
    (edge->color == GRAY && material_number != 7 << 3) &&
        (mats[extract_area_materials(material_number)]++);

    QUEUE_TRAVARSE_FINISH;
    return mats;
}

bool buy_settlement_to_player(PlayerPtr player,
                              GraphPtr graph,
                              const signed char cost[TOTAL_MATERIALS],
                              signed char bank[TOTAL_MATERIALS],
                              unsigned char index)
{
    printt("transfer_materials buy settlement to player");

    unsigned char *materials = svertex_to_materials(graph, index);
    print_vec(materials, TOTAL_MATERIALS);
    transfer_materials(player, bank, (signed char *)materials, true);

    free(materials);

    return true;
}

bool buy_settlement_from_player(PlayerPtr player,
                                GraphPtr graph,
                                const signed char cost[TOTAL_MATERIALS],
                                signed char bank[TOTAL_MATERIALS],
                                unsigned char index)
{
    putts("transfer_materials buy settlement from player");

    transfer_materials(player, bank, cost, false);

    return true;
}

bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const signed char cost[TOTAL_MATERIALS],
                    signed char bank[TOTAL_MATERIALS],
                    signed char transferMats, // -1 to player 1 from player
                    unsigned char index)
{

    bool conditions[2] = {transferMats == -1, transferMats == 1};
    unsigned char true_index = find_first_true_index(conditions, 2);
    bool (*transfer_actions[])(PlayerPtr, GraphPtr, const signed char[TOTAL_MATERIALS],
                               signed char[TOTAL_MATERIALS], unsigned char) = {buy_settlement_to_player, buy_settlement_from_player};

    (true_index < 2) && transfer_actions[true_index](player, graph, cost, bank, index);

    // change settlement color
    graph->vertices[index].color = player->color;
    player->victoryPoints++;

    avl_insert(&player->settlements, convert_unsigned_char_to_void_ptr(index), value_compare);

    player->victoryPoints++;
    player->amounts[SETTLEMENT]--;

    (graph->vertices[index].harbor) &&
        (player->harbors |= (1 << (graph->vertices[index].harbor - 1)));

    return true;
}

// use dev_card bot actions
void use_dev_knight(PlayerPtr player, int socket, GameState state)
{
    unsigned char size, area = moveRobberTo(player, state->graph, state->robberArea);
    player->knightUsed++;
    state->robberArea = area;

    // update bot developments cards
    player->developmentCards[KNIGHT_CARD]--;

    // update user
    unsigned char *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 6;
    buffer[1] = area;

    BOT_SEND_FREE(socket, size, buffer);

    socket_short_log(socket, 4);
}
void use_dev_point(PlayerPtr player, int socket, GameState state)
{
    // update client

    unsigned char size, *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 5;
    buffer[1] = player->developmentCards[VICTORY_POINTS_CARD];

    BOT_SEND_FREE(socket, size, buffer);
}
void use_dev_roads(PlayerPtr player, int socket, GameState state)
{

    putts("use_dev_roads");
    // get astrategy
    unsigned char offset, size, astindex = state->astIndexes[player->color - 1];

    // get buyable roads
    Stack stk;
    stack_init(&stk);

    buyableRoads(state->graph, &stk, player);

    unsigned short (*prioritiseRoad[])(GraphPtr, PlayerPtr, Heap[TOTAL_ASTRATEGIES], StackPtr) = {prioritiseWoodRoad, prioritiseWheatCardsRoad};

    for (offset = 0; offset < 2; offset++)
    {
        unsigned short road = prioritiseRoad[!!astindex](state->graph, player, state->astHeaps, &stk);
        putts("\t use_dev_roads before buy road");

        buy_road(player, state->graph, store[ROAD], state->bankMaterials, false,
                 road >> 8, road & 0xFF);
        putts("\t use_dev_roads after buy road");

        unsigned char *buffer = calloc(size = 3, sizeof(unsigned char));
        buffer[0] = 2;
        buffer[1] = road & 0xFF;
        buffer[2] = road >> 8;

        BOT_SEND_FREE(socket, size, buffer);
    }

    // update bot developments cards
    player->developmentCards[ROADS_CARD]--;

    socket_short_log(socket, 5);
}
void use_dev_yop(PlayerPtr player, int socket, GameState state)
{
    unsigned char count = 2, astindex = state->astIndexes[player->color - 1], *product,
                  *mats;
    bool condition, tried_everything = false;
    unsigned char *(*order[])(GraphPtr, PlayerPtr) = {
        woodMatsOrder, wheatMatsOrder, cardsMatsOrder};

    mats = (product = order[astindex](state->graph, player)) + TOTAL_MATERIALS;

    signed char *sub = vector_sub((signed char *)player->materials,
                                  (signed char *)product, TOTAL_MATERIALS);
    signed char *bank = vector_dup(state->bankMaterials, TOTAL_MATERIALS);

    unsigned char cost[TOTAL_MATERIALS] = {0};

    while (count && !tried_everything)
    {
        unsigned char min_index = vector_min_index(sub, TOTAL_MATERIALS);
        tried_everything = bank[min_index] == SIGNED_MAX_VALUE;

        condition = !!bank[min_index];
        cost[min_index]++;
        sub[min_index]++;

        condition &&
            (count--);

        !condition &&
            (bank[min_index] = SIGNED_MAX_VALUE);
    }
    putts("transfer_materials use dev yop");
    (!tried_everything) &&
        (transfer_materials(player, state->bankMaterials, (signed char *)cost, true),
         player->developmentCards[YEAR_OF_PLANT_CARD]--,
         socket_short_log(socket, 6), 1);

    free(sub);
    free(product);
}
void use_dev_monopol(PlayerPtr player, int socket, GameState state)
{
    unsigned char offset, astindex = state->astIndexes[player->color - 1], *product,
                          *mats;

    unsigned char *(*order[])(GraphPtr, PlayerPtr) = {
        woodMatsOrder, wheatMatsOrder, cardsMatsOrder};

    mats = (product = order[astindex](state->graph, player)) + TOTAL_MATERIALS;

    signed char *sub = vector_sub((signed char *)player->materials,
                                  (signed char *)product, TOTAL_MATERIALS);

    unsigned char min_index = vector_min_index(sub, TOTAL_MATERIALS);

    transfer_all_players_mats(state->players, player->color, state->num_of_players, min_index);

    free(sub);

    free(product);

    // update bot developments cards
    player->developmentCards[MONOPOL_CARD]--;

    socket_short_log(socket, 7);
}

// socket shortcuts
void socket_short_update(int socket)
{
    unsigned char size, *buffer = calloc(size = 1, sizeof(unsigned char));
    buffer[0] = 7;
    BOT_SEND_FREE(socket, size, buffer);
}
void socket_short_log(int socket, unsigned char log_type)
{
    unsigned char size, *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 8;
    buffer[1] = log_type;

    BOT_SEND_FREE(socket, size, buffer);
}