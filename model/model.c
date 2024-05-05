
#include "model.h"

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

unsigned char brand(unsigned char min, unsigned char max)
{
    return (babs(rand()) % (max - min)) + min;
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
    unsigned char offset, rand, temp, materials[AREAS - 1] = {WOOD, WOOD, WOOD, WOOD, WOOL, WOOL, WOOL, WOOL, WHEET, WHEET, WHEET, WHEET, BRICK, BRICK, BRICK, ORE, ORE, ORE},
                                                        numbers[AREAS - 1] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
    bool middle_or_after;

    // shuffle numbers and materials
    vec_shuffle((signed char *)materials, AREAS - 1);
    vec_shuffle((signed char *)numbers, AREAS - 1);

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
    unsigned char offset, mats[HARBOR_COUNT] = {WOOD, WOOL, WHEET, BRICK, ORE, GENERAL_DEAL, GENERAL_DEAL, GENERAL_DEAL, GENERAL_DEAL};

    vec_shuffle((signed char *)mats, HARBOR_COUNT);

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
void catab_players_init(PlayerPtr players, signed char size)
{
    while (--size >= 0)
    {
        players[size].color = size;
        players[size].amounts[ROAD] = 15;
        players[size].amounts[SETTLEMENT] = 5;
        players[size].amounts[CITY] = 4;
    }
    players[0].materials[0] = 5;
    players[0].materials[1] = 5;
    players[0].materials[2] = 5;
    players[0].materials[3] = 5;
    players[0].materials[4] = 5;
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
unsigned char *roll_dice(unsigned char *size,
                         PlayerPtr players,
                         GraphPtr graph,
                         signed char *bank)
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
        collect_materials(sum, players, graph, bank);
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
                              ? vec_sum(dev_bank, TOTAL_DEVELOPMENT_CARD)
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
        res = (unsigned char *)vec_dup(bank, (*size = count));
    }
    else if (!offset)
    {
        // is local
        // return every materials
        res = (unsigned char *)vec_dup((signed char *)manip(player), (*size = count));
    }
    else
    {
        // return only the count of the materials
        res = single_byte(size, vec_sum((signed char *)manip(player), count));
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
                                    signed char *bankMaterials, unsigned char offset)
{
    unsigned char *mats = inf_players_manip(size,
                                            players,
                                            offset,
                                            bankMaterials,
                                            TOTAL_MATERIALS,
                                            get_player_materials);
    printt("player mats = ");
    print_vec(mats, *size);
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
    return (unsigned char *)vec_dup((signed char *)player->amounts, (signed char)(*size = TOTAL_STORE - 1));
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

    avl_inorder(graph->vertices[from].edges, print_edge_offset);

    foundPtr = avl_search(graph->vertices[from].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = player->color;

    lookRec.offset = from;
    foundPtr = avl_search(graph->vertices[to].edges,
                          &lookRec, compare_edges_offset)
                   ->data;
    foundPtr->color = player->color;

    player->amounts[ROAD]--;
    return true;
}
signed char value_compare(const void *first, const void *second)
{
    return first - second;
}

unsigned char *svertex_to_materials(GraphPtr graph, signed char index)
{
    Queue que;
    EdgePtr edge;
    unsigned char size = 0, *mats = calloc(TOTAL_MATERIALS, sizeof(char));
    Node node = graph->vertices[index].edges;

    avl_inorder(node, print_edge_offset);
    puts("");
    queue_init(&que);
    enqueue(&que, node);

    while (!queue_empty(que))
    {
        size++;
        node = dequeue(&que);
        edge = (EdgePtr)node->data;
        if (node->left != NULL)
        {
            enqueue(&que, node->left);
        }
        if (node->right != NULL)
        {
            enqueue(&que, node->right);
        }

        if (edge->color == GRAY)
        {
            printt("\t\t\edge->offset = %d edge->color = %d material = %d index material = %d\n", edge->offset, edge->color, extract_area_materials(edge->vertex->material_number), extract_area_materials(graph->vertices[edge->offset].material_number));

            mats[extract_area_materials(edge->vertex->material_number)]++;
        }
    }
    printt("\t\t\tedgessize=%d\n", size);
    return mats;
}

bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const signed char cost[TOTAL_MATERIALS],
                    signed char bank[TOTAL_MATERIALS],
                    signed char transferMats, // -1 to player 1 from player
                    unsigned char index)
{
    printt("\t\t transferMats=%d\n", transferMats);
    switch (transferMats)
    {
    case 1:
        transfer_materials(player, bank, cost, false);
        break;
    case -1:
    {
        unsigned char *materials = svertex_to_materials(graph, index);
        transfer_materials(player, bank, (signed char *)materials, true);
        printt("\t\t moved materials to player index=%d ", index);
        print_vec(materials, TOTAL_MATERIALS);
        free(materials);
        break;
    }
    default:
        break;
    }

    // change settlement color
    graph->vertices[index].color = player->color;
    player->victoryPoints++;

    void *void_val = NULL;
    *(unsigned char *)&void_val = index;
    avl_insert(&player->settlements, void_val, value_compare);

    player->victoryPoints++;
    player->amounts[SETTLEMENT]--;

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

bool buy_developement(PlayerPtr player,
                      signed char bank[TOTAL_MATERIALS],
                      signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                      const signed char cost[TOTAL_MATERIALS])
{

    signed char *how_many = calloc(TOTAL_DEVELOPMENT_CARD, sizeof(signed char));
    signed char card_offset = -1, rand = brand(0, vec_sum(dev_bank, TOTAL_DEVELOPMENT_CARD));

    transfer_materials(player, bank, cost, false);

    while (rand > 0)
    {
        card_offset++;
        rand -= dev_bank[card_offset];
    }
    how_many[card_offset] = 1;

    transfer_dev_card(player, dev_bank, how_many, true);

    return true;
}

void update_achievements(GraphPtr graph, signed char achievements[TOTAL_ACHIEVEMENTS_CARD])
{
    
}

unsigned char *switch_action_store(unsigned char *size,
                                   signed char *params,
                                   GraphPtr graph,
                                   PlayerPtr player,
                                   signed char mat_bank[TOTAL_MATERIALS],
                                   signed char devcard_bank[TOTAL_DEVELOPMENT_CARD],
                                   const signed char store[TOTAL_STORE][TOTAL_MATERIALS], signed char achievements[TOTAL_ACHIEVEMENTS_CARD])
{
    unsigned char *res = calloc((*size = 1), sizeof(unsigned char));

    switch (params[0])
    {
    case 0:
        res[0] = buy_road(player,
                          graph,
                          store[ROAD],
                          mat_bank,
                          params[1],
                          params[2] + AREAS,
                          params[3] + AREAS);
        update_achievements(graph, achievements);
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

void collect_materials_area(VertexPtr area, PlayerPtr players, signed char *bank)
{
    Queue que;
    Node node;
    EdgePtr edge;

    signed char(*mats_to_players)[TOTAL_MATERIALS] =
        calloc(MAX_PLAYERS, sizeof(*mats_to_players));

    const unsigned char material = extract_area_materials(area->material_number);
    unsigned char player_offset;

    bool is_city;
    queue_init(&que);

    enqueue(&que, area->edges);
    while (!queue_empty(que))
    {
        node = dequeue(&que);
        edge = node->data;
        player_offset = edge->vertex->color & 0x0F;
        is_city = (edge->vertex->color >> 6) & 0x01;

        if (node->left != NULL)
        {
            enqueue(&que, node->left);
        }
        if (node->right != NULL)
        {
            enqueue(&que, node->right);
        }

        if (player_offset < MAX_PLAYERS)
        {
            printt("player_offset < MAX_PLAYERS! %d\n", player_offset);
        }
        mats_to_players[player_offset][material] += (player_offset < MAX_PLAYERS) * (1 + is_city);
    }

    for (player_offset = 0; player_offset < MAX_PLAYERS; player_offset++)
    {
        transfer_materials(players + player_offset, bank, mats_to_players[player_offset], true);
    }
}

void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank)
{
    // for each rolled num there can be between 1 to 2 vertecies;
    VertexPtr vertecies[2] = {NULL, NULL};
    unsigned char offset;
    signed char vertex;

    for (offset = 0, vertex = 0; offset < AREAS; offset++)
    {
        if (extract_area_number(graph->vertices[offset].material_number) == rolled_num)
        {
            vertecies[vertex++] = graph->vertices + offset;
        }
    }

    while (--vertex >= 0)
    {
        collect_materials_area(vertecies[vertex], players, bank);
    }
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
    sleep(0);
}

void handle_rest_turns(int socket,
                       unsigned char *turnOffset,
                       PlayerPtr players,
                       const unsigned num_of_players)
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
// math
signed char *vector_join(const signed char *first,
                         const signed char *second,
                         signed char size,
                         signed char (*func)(signed char first, signed char second))
{
    signed char *arr = calloc(size, sizeof(signed char));

    while (--size >= 0)
    {
        arr[size] = func(first[size], second[size]);
    }

    return arr;
}
signed char *vector_map(const signed char *first,
                        signed char size,
                        signed char (*func)(signed char))
{
    signed char *arr = calloc(size, sizeof(signed char));

    while (--size >= 0)
    {
        arr[size] = func(first[size]);
    }

    return arr;
}
signed char badd(signed char x, signed char y)
{
    return x + y;
}
signed char bminus(signed char x, signed char y)
{
    return x - y;
}
signed char bnegative(signed char x)
{
    return -x;
}
signed char *vector_add(const signed char *first,
                        const signed char *second,
                        signed char size)
{
    return vector_join(first, second, size, badd);
}

signed char *vector_sub(const signed char *first,
                        const signed char *second,
                        signed char size)
{
    return vector_join(first, second, size, bminus);
}
signed char *vector_neg(const signed char *arr,
                        signed char size)
{
    return vector_map(arr, size, bnegative);
}
bool vector_any(const signed char *arr, signed char size, bool (*condition)(signed char))
{
    while (--size >= 0 && !condition(arr[size]))
        ;

    return size >= 0;
}
bool vector_all(const signed char *arr, signed char size, bool (*condition)(signed char))
{
    while (--size >= 0 && condition(arr[size]))
        ;

    return size < 0;
}
void vector_cpy(signed char *dest, signed char *src, signed char size)
{
    while (--size >= 0)
    {
        dest[size] = src[size];
    }
}

void vector_val(signed char *dest, signed char size, signed char val)
{
    unsigned char offset;

    while (--size >= 0)
    {
        dest[offset] = val;
    }
}
signed char vec_sum(signed char *arr, signed char size)
{
    unsigned char val;

    while (--size >= 0)
    {
        val += arr[size];
    }
    return val;
}
signed char *vec_dup(signed char *arr, signed char size)
{
    signed char *new = malloc(sizeof(signed char) * size);
    while (--size >= 0)
    {
        new[size] = arr[size];
    }
    return new;
}
void bswap(signed char *x, signed char *y)
{
    signed char temp = *y;
    *y = *x;
    *x = temp;
}
void vec_shuffle(signed char *arr, unsigned char size)
{
    unsigned char offset;
    for (offset = 0; offset < size; offset++)
    {
        bswap(arr + offset, arr + brand(0, size));
    }
}