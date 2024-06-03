
#include "../headers/model.h"

// extract data functions
unsigned char extract_area_materials(unsigned char material_number)
{
    return material_number & 0x07;
}

unsigned char extract_area_number(unsigned char material_number)
{
    return material_number >> 3;
}

// helper function
void print_vec(unsigned char *arr, signed char size)
{
    unsigned char offset;
    printf("[");

    for (offset = 0; offset < size; offset++)
    {
        printf(offset ? ", %d" : "%d", arr[offset]);
    }

    printf("]\n");
}

void catan_edges(GraphPtr graph)
{
    FILE *file;

#ifdef _WIN32
    fopen_s(&file, "edges.byte", "rb");
#else
    file = fopen("edges.byte", "rb");
#endif

    if (!file)
    {
        perror("could not open edges.byte!");
        exit(EXIT_FAILURE);
    }

    signed char elements[2];
    // printt("building graph\n");
    while (!feof(file))
    {
        fread(elements, sizeof(signed char), 2, file);

        const unsigned char from = elements[0], to = elements[1];
        const bool is_connecting = (bmin(from, to) < AREAS);

        graph_join(graph, from, to, BLACK + is_connecting);
    }
    // printt("done graph\n");

    fclose(file);
}

void catan_lands(GraphPtr graph)
{
    unsigned char offset, rand, temp, materials[AREAS - 1] = {WOOD, WOOD, WOOD, WOOD, WOOL, WOOL, WOOL, WOOL, WHEAT, WHEAT, WHEAT, WHEAT, BRICK, BRICK, BRICK, ORE, ORE, ORE},
                                                        numbers[AREAS - 1] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    bool middle_or_after;

    // shuffle numbers and materials
    vector_shuffle((signed char *)materials, AREAS - 1);
    vector_shuffle((signed char *)numbers, AREAS - 1);

    for (offset = 0; offset < AREAS - 1; offset++)
    {
        middle_or_after = offset >= 9;

        graph->vertices[offset + middle_or_after].material_number = numbers[offset] << 3 | materials[offset];
        graph->vertices[offset + middle_or_after].color = GRAY;
    }

    graph->vertices[9].material_number = 7 << 3;
    graph->vertices[9].color = GRAY;
}

void catan_harbors(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2])
{
    unsigned char offset, mats[HARBOR_COUNT] = {WOOD, WOOL, WHEAT, BRICK, ORE, GENERAL_DEAL, GENERAL_DEAL, GENERAL_DEAL, GENERAL_DEAL};

    vector_shuffle((signed char *)mats, HARBOR_COUNT);

    for (offset = 0; offset < HARBOR_COUNT; offset++)
    {
        graph->vertices[harbors[offset * 2] + AREAS].harbor =
            graph->vertices[harbors[offset * 2 + 1] + AREAS].harbor =
                mats[offset] + 1;
    }
}

void catan_settle_vertecies(GraphPtr graph)
{
    unsigned char offset;

    for (offset = 0; offset < VERTECIES; offset++)
    {
        graph->vertices[offset + AREAS].color = BLACK;
    }
}

void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2])
{
    catan_edges(graph);
    catan_lands(graph);

    catan_settle_vertecies(graph);
    catan_harbors(graph, harbors);
}
void catan_players_init(PlayerPtr players, signed char size)
{
    while (--size >= 0)
    {
        players[size].color = size;
        players[size].amounts[ROAD] = 15;
        players[size].amounts[SETTLEMENT] = 5;
        players[size].amounts[CITY] = 4;

        // FIXME: remove
        players[size].materials[0] = 10;
        players[size].materials[1] = 10;
        players[size].materials[2] = 10;
        players[size].materials[3] = 10;
        players[size].materials[4] = 10;

        players[size].developmentCards[0] = 10;
        players[size].developmentCards[1] = 10;
        players[size].developmentCards[2] = 10;
        players[size].developmentCards[3] = 10;
        players[size].developmentCards[4] = 10;
    }
}
unsigned char *areas_numbers(unsigned char *size, GraphPtr graph)
{
    unsigned char *arr = malloc(sizeof(signed char) * (*size = AREAS)), offset;

    for (offset = 0; offset < AREAS; offset++)
        arr[offset] = graph->vertices[offset].material_number;

    return arr;
}
unsigned char *harbors_numbers(unsigned char *size,
                               GraphPtr graph,
                               unsigned char harbors[HARBOR_COUNT * 2])
{

    unsigned char offset, general_deal_counter, mat, mat_index,
        *harbors_numbs = malloc((*size = HARBOR_COUNT * 2) * sizeof(unsigned char));

    for (offset = 0, general_deal_counter = 0; offset < HARBOR_COUNT; offset++)
    {
        mat = mat_index = graph->vertices[harbors[offset * 2] + AREAS].harbor - 1;

        if (mat == GENERAL_DEAL)
            mat_index += general_deal_counter++;

        harbors_numbs[mat_index * 2] = harbors[offset * 2];
        harbors_numbs[mat_index * 2 + 1] = harbors[offset * 2 + 1];
    }
    print_vec(harbors_numbs, HARBOR_COUNT * 2);
    return harbors_numbs;
}

unsigned char *players_around_area(unsigned char *size,
                                   unsigned char vertex_area,
                                   GraphPtr graph)
{
    // each bit represent if the player is around the area or not
    unsigned char playerBits = 0;

    VertexPtr area = graph->vertices + vertex_area;
    VertexPtr dest = graph->vertices + vertex_area;

    EdgePtr edge;
    Node node;

    QUEUE_TRAVARSE(area->edges, node);

    edge = node->data;
    dest = edge->vertex;

    if (dest->color < BLACK)
    {
        playerBits |= 1 << dest->color;
    }

    QUEUE_TRAVARSE_FINISH;

    return single_byte(size, playerBits);
}
unsigned char *roll_dice(unsigned char *size,
                         PlayerPtr players,
                         GraphPtr graph,
                         signed char *bank,
                         const unsigned char robberArea)
{
    unsigned char *arr, dice, sum;

    sum = dice = brand(1, 7);
    sum += (dice = brand(1, 7));
    arr = single_byte(size, (dice << 4) | (sum - dice));

    printt("\trolled number %hhu while arr %hhd (%hhd, %hhd)\n", sum, arr[0], sum - dice, dice);
    switch (sum)
    {
    case 7:
        break;

    default:
        collect_materials(sum, players, graph, bank, robberArea);
        break;
    }

    return arr;
}
bool bis_neg(signed char x)
{
    return x < 0;
}
unsigned char *inf_player_actionable(unsigned char *size,
                                     PlayerPtr player,
                                     signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                                     const signed char store[TOTAL_STORE][TOTAL_MATERIALS])
{
    unsigned char *arr = calloc((*size = 1), sizeof(signed char)), offset;

    for (offset = 0; offset < TOTAL_STORE; offset++)
    {
        signed char *arr_res = vector_sub((signed char *)player->materials,
                                          store[offset], TOTAL_MATERIALS);
        bool not_neg = !vector_any(arr_res, TOTAL_MATERIALS, bis_neg);
        bool has_amount = offset == DEVELOPMENT_CARD
                              ? vector_sum(dev_bank, TOTAL_DEVELOPMENT_CARD)
                              : player->amounts[offset];

        arr[0] |= (not_neg && has_amount) << offset;
        free(arr_res);
    }

    return arr;
}

unsigned char *inf_players_manip(unsigned char *size,
                                 PlayerPtr players,
                                 unsigned char offset,
                                 signed char *bank,
                                 unsigned char count,
                                 unsigned char *(*manip)(PlayerPtr))
{
    PlayerPtr player = players + offset;
    unsigned char *res;

    bool _is_bank = offset == MAX_PLAYERS,
         _is_local = !offset;

    if (offset == MAX_PLAYERS)
    {
        // is bank
        res = (unsigned char *)vector_dup(bank, (*size = count));
    }
    else if (!offset)
    {
        // is local
        // return every materials
        res = (unsigned char *)vector_dup((signed char *)manip(player), (*size = count));
    }
    else
    {
        // return only the count of the materials
        res = single_byte(size, vector_sum((signed char *)manip(player), count));
    }

    return res;
}

unsigned char *get_player_materials(PlayerPtr player)
{
    return player->materials;
}

unsigned char *get_player_devcards(PlayerPtr player)
{
    return player->developmentCards;
}

unsigned char *inf_player_materials(unsigned char *size,
                                    PlayerPtr players,
                                    signed char *bankMaterials,
                                    unsigned char offset)
{
    unsigned char *mats = inf_players_manip(size,
                                            players,
                                            offset,
                                            bankMaterials,
                                            TOTAL_MATERIALS,
                                            get_player_materials);
    return mats;
}
unsigned char *inf_player_devcards(unsigned char *size,
                                   PlayerPtr players,
                                   signed char *bankDevelopment,
                                   unsigned char offset)
{
    return inf_players_manip(size,
                             players,
                             offset,
                             bankDevelopment,
                             TOTAL_DEVELOPMENT_CARD,
                             get_player_devcards);
}

unsigned char *inf_player_amounts(unsigned char *size, PlayerPtr player)
{
    return (unsigned char *)vector_dup((signed char *)player->amounts, (signed char)(*size = TOTAL_STORE - 1));
}
unsigned char *single_byte(unsigned char *size, signed char value)
{
    unsigned char *res = malloc(sizeof(unsigned char) * (*size = 1));
    *res = value;
    return res;
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

signed char compare_edges_offset(const void *first, const void *second)
{
    return ((EdgePtr)first)->offset - ((EdgePtr)second)->offset;
}
void print_edge_offset(const void *ptr)
{
    EdgePtr edge = (EdgePtr)ptr;
    printf(" %d ", edge->offset);
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
    if (transferMats)
        transfer_materials(player, bank, cost, false);

    EdgePtr foundPtr;
    EdgeRec lookRec;

    lookRec.offset = to;

    foundPtr = avl_search(graph->vertices[from].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = player->color;
    avl_insert(&player->roads, foundPtr, value_compare);

    lookRec.offset = from;
    foundPtr = avl_search(graph->vertices[to].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = player->color;
    avl_insert(&player->roads, foundPtr, value_compare);

    player->amounts[ROAD]--;
    return true;
}

unsigned char *svertex_to_materials(GraphPtr graph, signed char index)
{
    Queue que;
    EdgePtr edge;
    unsigned char size = 0, *mats = calloc(TOTAL_MATERIALS, sizeof(char));
    Node node = graph->vertices[index].edges;

    QUEUE_TRAVARSE(node, node);
    size++;
    edge = (EdgePtr)node->data;

    if (edge->color == GRAY)
    {
        mats[extract_area_materials(edge->vertex->material_number)]++;
    }
    QUEUE_TRAVARSE_FINISH;
    return mats;
}

bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const signed char cost[TOTAL_MATERIALS],
                    signed char bank[TOTAL_MATERIALS],
                    signed char transferMats, // -1 to player 1 from player
                    unsigned char index)
{
    switch (transferMats)
    {
    case 1:
        transfer_materials(player, bank, cost, false);
        break;
    case -1:
    {
        unsigned char *materials = svertex_to_materials(graph, index);
        transfer_materials(player, bank, (signed char *)materials, true);
        free(materials);
        break;
    }
    default:
        break;
    }

    // change settlement color
    graph->vertices[index].color = player->color;
    player->victoryPoints++;

    avl_insert(&player->settlements, convert_unsigned_char_to_void_ptr(index), value_compare);

    player->victoryPoints++;
    player->amounts[SETTLEMENT]--;

    if (graph->vertices[index].harbor)
        player->harbors |= (1 << (graph->vertices[index].harbor - 1));

    return true;
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

// O(E+V)
signed char dfs(GraphPtr graph, unsigned char vertex_offset, unsigned char targetColor, bool *visited)
{
    VertexPtr vertex = graph->vertices + vertex_offset;
    if (vertex == NULL || vertex->color == GRAY || (visited && visited[vertex_offset - AREAS]))
    {
        return 0;
    }

    if (visited)
    {
        visited[vertex_offset - AREAS] = true;
    }
    signed char max_child_length = 0;

    Node node = vertex->edges;

    EdgePtr edge;

    QUEUE_TRAVARSE(node, node);
    edge = node->data;

    if (edge->color == targetColor)
    {
        max_child_length = bmax(max_child_length, dfs(graph, edge->offset, targetColor, visited));
    }
    QUEUE_TRAVARSE_FINISH;

    return max_child_length + 1;
}

// O(CE+CVE+C) | C is negligible
signed char update_longest_road(GraphPtr graph, signed char *longest_road_achievement)
{
    unsigned char color_offset, vertex_offset, max_road_color = BLACK;
    signed char temp_length, max_road_length = 0, max_road_per_color[MAX_PLAYERS] = {0}, old_val;

    for (color_offset = 0; color_offset < MAX_PLAYERS; color_offset++)
    {
        for (vertex_offset = 0; vertex_offset < VERTECIES; vertex_offset++)
        {
            bool *visited = calloc(VERTECIES, sizeof(bool));
            max_road_per_color[color_offset] = bmax(max_road_per_color[color_offset],
                                                    dfs(graph,
                                                        vertex_offset + AREAS,
                                                        color_offset,
                                                        visited) -
                                                        1);
            free(visited);
        }

        if (max_road_per_color[color_offset] > max_road_length)
        {
            max_road_length = max_road_per_color[color_offset];
            max_road_color = color_offset;
        }
    }

    if (max_road_length < 5)
    {
        *longest_road_achievement = -1;
    }
    else if (vector_count(max_road_per_color,
                          MAX_PLAYERS,
                          max_road_length) == 1)
    {
        *longest_road_achievement = max_road_color;
    }

    print_vec((unsigned char *)max_road_per_color, MAX_PLAYERS);

    return *longest_road_achievement;
}

unsigned char *switch_action_store(unsigned char *size,
                                   signed char *params,
                                   GraphPtr graph,
                                   PlayerPtr player,
                                   signed char mat_bank[TOTAL_MATERIALS],
                                   signed char devcard_bank[TOTAL_DEVELOPMENT_CARD],
                                   const signed char store[TOTAL_STORE][TOTAL_MATERIALS], signed char *longest_road_achievement)
{
    unsigned char *res = calloc((*size = 1), sizeof(unsigned char));

    switch (params[0])
    {
    case 0:
        buy_road(player,
                 graph,
                 store[ROAD],
                 mat_bank,
                 params[1],
                 params[2] + AREAS,
                 params[3] + AREAS);
        res[0] = update_longest_road(graph, longest_road_achievement);
        break;
    case 1:
        res[0] = buy_settlement(player,
                                graph,
                                store[SETTLEMENT],
                                mat_bank,
                                params[1],
                                params[2] + AREAS);
        break;
    case 2:
        res[0] = buy_city(player,
                          graph,
                          store[CITY],
                          mat_bank,
                          params[2] + AREAS);
        break;
    case 3:
        res[0] = buy_developement(player,
                                  mat_bank,
                                  devcard_bank,
                                  store[DEVELOPMENT_CARD]);
        break;
    default:
        res[0] = false;
        break;
    }
    return res;
}

void transfer_all_players_mats(PlayerPtr players,
                               unsigned char player_index,
                               signed char num_of_players,
                               unsigned char mat)
{
    PlayerPtr other;
    while (--num_of_players >= 0)
    {
        other = players + num_of_players;
        signed char mats_to_transfer[TOTAL_MATERIALS] = {0};

        if (num_of_players != player_index)
        {
            mats_to_transfer[mat] = other->materials[mat];
            transfer_materials(players + player_index, (signed char *)other->materials, mats_to_transfer, true);
        }
    }
}

unsigned char *switch_dev_card(unsigned char *size,
                               PlayerPtr players,
                               unsigned char num_of_players,
                               signed char *bankMaterials,
                               signed char *params)
{
    switch (params[0])
    {
    case 3:
    {
        signed char mats_to_transfer[TOTAL_MATERIALS] = {0};
        mats_to_transfer[params[1]]++;
        mats_to_transfer[params[2]]++;
        transfer_materials(players, bankMaterials, mats_to_transfer, true);
    }
    break;
    case 4:
        transfer_all_players_mats(players, 0, (signed char)num_of_players, params[1]);
    }
    players->developmentCards[params[0]]--;

    return single_byte(size, 0);
}
void collect_materials_area(VertexPtr area, PlayerPtr players, signed char *bank)
{
    Node node;
    EdgePtr edge;

    signed char(*mats_to_players)[TOTAL_MATERIALS] =
        calloc(MAX_PLAYERS, sizeof(*mats_to_players));

    const unsigned char material = extract_area_materials(area->material_number);
    unsigned char player_offset;

    bool is_city;

    QUEUE_TRAVARSE(area->edges, node);

    edge = node->data;
    player_offset = edge->vertex->color & 0x0F;
    is_city = (edge->vertex->color >> 6) & 0x01;
    mats_to_players[player_offset][material] += (player_offset < MAX_PLAYERS) * (1 + is_city);

    QUEUE_TRAVARSE_FINISH;

    for (player_offset = 0; player_offset < MAX_PLAYERS; player_offset++)
    {
        transfer_materials(players + player_offset, bank, mats_to_players[player_offset], true);
    }
}

void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank,
                       const unsigned char robberArea)
{
    // for each rolled num there can be between 1 to 2 vertecies;
    VertexPtr vertecies[2] = {NULL, NULL};
    unsigned char offset;
    signed char vertex;

    for (offset = 0, vertex = 0; offset < AREAS; offset++)
    {
        if (extract_area_number(graph->vertices[offset].material_number) == rolled_num && robberArea != offset)
        {
            vertecies[vertex++] = graph->vertices + offset;
        }
    }

    while (--vertex >= 0)
    {
        collect_materials_area(vertecies[vertex], players, bank);
    }
}
signed char update_biggest_army(PlayerPtr players,
                                signed char num_of_players,
                                signed char *biggest_army_achievement)
{
    unsigned char maxIndex = 0;

    while (--num_of_players >= 0)
    {
        if (players[num_of_players].knightUsed > players[maxIndex].knightUsed)
        {
            maxIndex = num_of_players;
        }
    }

    if (players[maxIndex].knightUsed >= 3)
    {
        *biggest_army_achievement = maxIndex;
    }
    else
    {
        *biggest_army_achievement = -1;
    }
    printt("\t\tmax = %d (%d)\n", players[maxIndex].knightUsed, maxIndex);
    return *biggest_army_achievement;
}
unsigned char *move_robber(unsigned char *size,
                           PlayerPtr players,
                           const unsigned char player_index,
                           unsigned char *robberArea,
                           signed char *params,
                           signed char *biggest_army_achievement,
                           const unsigned char num_of_players)
{
    unsigned char *res = single_byte(size, -1);
    unsigned char target_index = params[0]; // if got 7!
    *robberArea = params[1];

    switch (target_index)
    {
    case 4:
        break;
    case 3:
    case 2:
    case 1:
    {
        unsigned char cost[TOTAL_MATERIALS] = {0};
        cost[random_index_by_vals(TOTAL_MATERIALS, (signed char *)cost)]++;
        transfer_materials(players + player_index,
                           (signed char *)players[target_index].materials,
                           (signed char *)cost, true);
        printt("steal mode player index: %d\n", target_index);
    }
    break;

    case 0:
        players[player_index].knightUsed++;
        printt("pullin up knight!! %d\n", players[player_index].knightUsed);
        break;
    }

    res[0] = update_biggest_army(players, num_of_players,
                                 biggest_army_achievement);
    printt("returning %hhd\n", *res);
    return res;
}
unsigned char *drop_materials(unsigned char *size, PlayerPtr player, signed char *mats)
{
    signed char *sub = vector_sub((signed char *)player->materials,
                                  mats, TOTAL_MATERIALS);
    vector_cpy((signed char *)player->materials, sub, TOTAL_MATERIALS);
    return single_byte(size, 0);
}
// bots

void bot_inits(PlayerPtr players, unsigned char num_of_players)
{
    while (--num_of_players)
    {
    }
    // TODO: pick approach!
}

void bot_plays(PlayerPtr player, int socket)
{

    // TODO: play by selected approach and update client player!
    usleep(500000);
}

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
                       unsigned char *astIndexes)
{
    (*turnOffset)++;
    signed char *_buff = malloc(sizeof(char) * 1000);

    unsigned char turnColor, size;

    // while the turn color its not WHITE
    while ((turnColor = (*turnOffset) % num_of_players))
    {

        bot_plays(players + turnColor, socket);

        // printt("\tbot number %d done playing!", turnColor);
        (*turnOffset)++;

        _buff[0] = 0;
        _buff[1] = (*turnOffset) % num_of_players;
        size = 2;

        send(socket, &size, 1, 0);
        send(socket, _buff, size, 0);
    }
}
