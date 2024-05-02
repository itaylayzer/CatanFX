
#include "model.h"

// helper function
void print_vec(unsigned char *arr, char size)
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

    char elements[2];
    // printt("building graph\n");
    while (!feof(file))
    {
        fread(elements, sizeof(char), 2, file);

        const unsigned char from = elements[0], to = elements[1];
        if (to == 146)
            continue;
        // printf("\t%hhu %hhu\n", from, to);
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
    vec_shuffle((char *)materials, AREAS - 1);
    vec_shuffle((char *)numbers, AREAS - 1);

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

    vec_shuffle((char *)mats, HARBOR_COUNT);

    for (offset = 0; offset < HARBOR_COUNT; offset++)
    {
        graph->vertices[harbors[offset * 2] + AREAS].harbor =
            graph->vertices[harbors[offset * 2 + 1] + AREAS].harbor =
                mats[offset] + 1;
    }
}

void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2])
{
    catan_edges(graph);
    catan_lands(graph);

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
}
unsigned char *areas_numbers(unsigned char *size, GraphPtr graph)
{
    unsigned char *arr = malloc(sizeof(char) * (*size = AREAS)), offset;

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
                         char *bank)
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
bool bis_neg(char x)
{
    return x < 0;
}
unsigned char *inf_player_actionable(unsigned char *size,
                                     PlayerPtr player,
                                     char dev_bank[TOTAL_DEVELOPMENT_CARD],
                                     const char store[TOTAL_STORE][TOTAL_MATERIALS])
{
    unsigned char *arr = calloc((*size = 1), sizeof(char)), offset;

    for (offset = 0; offset < TOTAL_STORE; offset++)
    {
        char *arr_res = vector_sub((char *)player->materials,
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
                                 unsigned char count,
                                 unsigned char *(*manipulation)(PlayerPtr))
{
    PlayerPtr player = players + offset;
    unsigned char *res;
    if (offset)
    {
        // return only the count of the materials
        res = single_byte(size, vec_sum((char *)manipulation(player), count));
    }
    else
    {
        // return every materials
        res = (unsigned char *)vec_dup((char *)manipulation(player), (*size = count));
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
                                    unsigned char offset)
{
    return inf_players_manip(size,
                             players,
                             offset,
                             TOTAL_MATERIALS,
                             get_player_materials);
}
unsigned char *inf_player_devcards(unsigned char *size,
                                   PlayerPtr players,
                                   unsigned char offset)
{
    return inf_players_manip(size,
                             players,
                             offset,
                             TOTAL_DEVELOPMENT_CARD,
                             get_player_devcards);
}
unsigned char *single_byte(unsigned char *size, char value)
{
    unsigned char *res = malloc(sizeof(unsigned char) * (*size = 1));
    *res = value;
    return res;
}

void transfer_materials(PlayerPtr player,
                        char bank[TOTAL_MATERIALS],
                        const char cost[TOTAL_MATERIALS],
                        bool to_player)
{
    char *player_mats = (char *)player->materials;

    // remove cost from player materials
    char *new_materials = (to_player ? vector_add : vector_sub)(player_mats, cost, TOTAL_MATERIALS);
    vector_cpy(player_mats, (char *)new_materials, TOTAL_MATERIALS);
    free(new_materials);

    // update bank
    new_materials = (to_player ? vector_sub : vector_add)(bank, cost, TOTAL_MATERIALS);
    vector_cpy(bank, (char *)new_materials, TOTAL_MATERIALS);
    free(new_materials);
}

void transfer_dev_card(PlayerPtr player,
                       char bank[TOTAL_DEVELOPMENT_CARD],
                       const char *cost,
                       bool to_player)
{
    char *player_devs = (char *)player->developmentCards;

    // remove cost from player materials
    char *new_materials =
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

bool buy_road(PlayerPtr player,
              GraphPtr graph,
              const char cost[TOTAL_MATERIALS],
              char bank[TOTAL_MATERIALS],
              unsigned char from,
              unsigned char to)
{
    transfer_materials(player, bank, cost, false);

    EdgePtr tempPtr;
    EdgeRec tempRec;

    tempRec.offset = to;
    tempPtr = avl_search(graph->vertices[from].edges,
                         &tempRec, compare_edges_offset)
                  ->data;
    tempPtr->color = player->color;

    tempRec.offset = from;
    tempPtr = avl_search(graph->vertices[from].edges,
                         &tempRec, compare_edges_offset)
                  ->data;
    tempPtr->color = player->color;

    // TODO: Now check who has the most longest path

    player->amounts[ROAD]--;
    return true;
}
char value_compare(const void *first, const void *second)
{
    return first - second;
}
bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const char cost[TOTAL_MATERIALS],
                    char bank[TOTAL_MATERIALS],
                    unsigned char index)
{

    transfer_materials(player, bank, cost, false);

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
              const char cost[TOTAL_MATERIALS],
              char bank[TOTAL_MATERIALS],
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
                      char bank[TOTAL_MATERIALS],
                      char dev_bank[TOTAL_DEVELOPMENT_CARD],
                      const char cost[TOTAL_MATERIALS])
{

    char *how_many = calloc(TOTAL_DEVELOPMENT_CARD, sizeof(char));
    char card_offset = -1, rand = brand(0, vec_sum(dev_bank, TOTAL_DEVELOPMENT_CARD));

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

unsigned char *switch_action_store(unsigned char *size,
                                   unsigned char *params,
                                   GraphPtr graph,
                                   PlayerPtr player,
                                   char mat_bank[TOTAL_MATERIALS],
                                   char devcard_bank[TOTAL_DEVELOPMENT_CARD],
                                   const char store[TOTAL_STORE][TOTAL_MATERIALS])
{
    unsigned char *res = calloc((*size = 1), sizeof(unsigned char));
    switch (params[0])
    {
    case 0:
        res[0] = buy_road(player, graph, store[ROAD], mat_bank, params[1], params[2]);
        break;
    case 1:
        res[0] = buy_settlement(player, graph, store[SETTLEMENT], mat_bank, params[1]);
        break;
    case 2:
        res[0] = buy_city(player, graph, store[CITY], mat_bank, params[1]);
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

void collect_materials_area(VertexPtr area, PlayerPtr players, char *bank)
{
    Queue que;
    Node node;
    EdgePtr edge;

    const unsigned char material = area->material_number & 0x07;
    unsigned char player_offset, cost[TOTAL_MATERIALS];

    bool is_city;
    queue_init(&que);

    enqueue(&que, area->edges);
    while (!queue_empty(que))
    {
        vector_val((char *)cost, TOTAL_MATERIALS, 0);

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
            cost[material] = 1 + is_city;
            transfer_materials(players + player_offset, bank, (char *)cost, true);
        }
    }
}

void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       char *bank)
{
    // for each rolled num there can be between 1 to 2 vertecies;
    VertexPtr vertecies[2] = {NULL, NULL};
    unsigned char offset;
    signed char vertex;

    for (offset = 0, vertex = 0; offset < AREAS; offset++)
    {
        if (graph->vertices[offset].material_number >> 3 == rolled_num)
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
    sleep(1);
}

void handle_rest_turns(int socket,
                       unsigned char *turnOffset,
                       PlayerPtr players,
                       const unsigned num_of_players)
{
    (*turnOffset)++;

    unsigned char turnColor;

    // while the turn color its not WHITE
    while ((turnColor = (*turnOffset) / num_of_players))
    {
        bot_plays(players + turnColor, socket);
        (*turnOffset)++;
    }
}
// math
char *vector_join(const char *first,
                  const char *second,
                  signed char size,
                  char (*func)(char first, char second))
{
    char *arr = calloc(size, sizeof(char));

    while (--size >= 0)
    {
        arr[size] = func(first[size], second[size]);
    }

    return arr;
}
char *vector_map(const char *first,
                 signed char size,
                 char (*func)(char))
{
    char *arr = calloc(size, sizeof(char));

    while (--size >= 0)
    {
        arr[size] = func(first[size]);
    }

    return arr;
}
char badd(char x, char y)
{
    return x + y;
}
char bminus(char x, char y)
{
    return x - y;
}
char bnegative(char x)
{
    return -x;
}
char *vector_add(const char *first,
                 const char *second,
                 char size)
{
    return vector_join(first, second, size, badd);
}

char *vector_sub(const char *first,
                 const char *second,
                 char size)
{
    return vector_join(first, second, size, bminus);
}
char *vector_neg(const char *arr,
                 char size)
{
    return vector_map(arr, size, bnegative);
}
bool vector_any(const char *arr, signed char size, bool (*condition)(char))
{
    while (--size >= 0 && !condition(arr[size]))
        ;

    return size >= 0;
}
bool vector_all(const char *arr, signed char size, bool (*condition)(char))
{
    while (--size >= 0 && condition(arr[size]))
        ;

    return size < 0;
}
void vector_cpy(char *dest, char *src, signed char size)
{
    while (--size >= 0)
    {
        dest[size] = src[size];
    }
}

void vector_val(char *dest, signed char size, char val)
{
    unsigned char offset;

    while (--size >= 0)
    {
        dest[offset] = val;
    }
}
char vec_sum(char *arr, signed char size)
{
    unsigned char val;

    while (--size >= 0)
    {
        val += arr[size];
    }
    return val;
}
char *vec_dup(char *arr, signed char size)
{
    char *new = malloc(sizeof(char) * size);
    while (--size >= 0)
    {
        new[size] = arr[size];
    }
    return new;
}
void bswap(char *x, char *y)
{
    char temp = *y;
    *y = *x;
    *x = temp;
}
void vec_shuffle(char *arr, unsigned char size)
{
    unsigned char offset;
    for (offset = 0; offset < size; offset++)
    {
        bswap(arr + offset, arr + brand(0, size));
    }
}