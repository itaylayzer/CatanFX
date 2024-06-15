
#include "../headers/model.h"

// extract data functions

unsigned char extract_area_number(unsigned char material_number)
{
    return material_number >> 3;
}

void catan_edges(GraphPtr graph)
{
    FILE *file;

#ifdef _WIN32
    fopen_s(&file, "edges.byte", "rb");
#else
    file = fopen("edges.byte", "rb");
#endif

    assert(file && "could not open edges.byte!");

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

        // // FIXME: remove
        // players[size].materials[0] = 10;
        // players[size].materials[1] = 10;
        // players[size].materials[2] = 10;
        // players[size].materials[3] = 10;
        // players[size].materials[4] = 10;
        // players[size].developmentCards[0] = 10;
        // players[size].developmentCards[1] = 10;
        // players[size].developmentCards[2] = 10;
        // players[size].developmentCards[3] = 10;
        // players[size].developmentCards[4] = 10;
    }
}
unsigned char *areas_numbers(unsigned char *size, signed char *params,
                             int socket, GameState state)
{
    unsigned char *arr = malloc(sizeof(signed char) * (*size = AREAS)), offset;

    for (offset = 0; offset < AREAS; offset++)
        arr[offset] = state->graph->vertices[offset].material_number;

    return arr;
}
unsigned char *harbors_numbers(unsigned char *size, signed char *params,
                               int socket, GameState state)
{

    unsigned char offset, general_deal_counter, mat, mat_index,
        *harbors_numbs = malloc((*size = HARBOR_COUNT * 2) * sizeof(unsigned char));

    for (offset = 0, general_deal_counter = 0; offset < HARBOR_COUNT; offset++)
    {
        mat = mat_index = state->graph->vertices[state->harbors[offset * 2] + AREAS].harbor - 1;

        mat == GENERAL_DEAL && (mat_index += general_deal_counter++);

        harbors_numbs[mat_index * 2] = state->harbors[offset * 2];
        harbors_numbs[mat_index * 2 + 1] = state->harbors[offset * 2 + 1];
    }
    print_vec(harbors_numbs, HARBOR_COUNT * 2);
    return harbors_numbs;
}

unsigned char *players_around_area(unsigned char *size, signed char *params,
                                   int socket, GameState state)
{
    // each bit represent if the player is around the area or not
    unsigned char playerBits = 0;

    VertexPtr area = state->graph->vertices + params[0];
    VertexPtr dest = state->graph->vertices + params[0];

    EdgePtr edge;
    Node node;

    QUEUE_TRAVARSE(area->edges, node);

    edge = node->data;
    dest = edge->vertex;

    (dest->color < BLACK) &&
        (playerBits |= 1 << dest->color);

    QUEUE_TRAVARSE_FINISH;

    return single_byte(size, playerBits);
}

unsigned char *roll_dice(unsigned char *size, signed char *params,
                         int socket, GameState state)
{
    unsigned char *arr, dice, sum;

    sum = /*dice = 3;*/ brand(1, 7);
    sum += /*(dice = 4); */ dice = brand(1, 7);
    arr = single_byte(size, (dice << 4) | (sum - dice));

    printt("\trolled number %hhu while arr %hhd (%hhd, %hhd)\n", sum, arr[0], sum - dice, dice);
    switch (sum)
    {
    case 7:
        break;

    default:
        collect_materials(sum, state->players, state->graph, state->bankMaterials, state->robberArea);
        break;
    }

    return arr;
}
bool bis_neg(signed char x)
{
    return x < 0;
}
unsigned char *inf_player_actionable(unsigned char *size, signed char *params,
                                     int socket, GameState state)
{
    unsigned char *arr = calloc((*size = 2), sizeof(signed char)), offset;

    for (offset = 0; offset < TOTAL_STORE; offset++)
    {

        bool not_neg = !(vector_manip_condition(
                             (signed char *)state->players[params[0]].materials,
                             store[offset], TOTAL_MATERIALS,
                             vector_sub, bis_neg) >>
                         1);

        bool has_amount = offset == DEVELOPMENT_CARD
                              ? vector_sum(state->bankDevelopments, TOTAL_DEVELOPMENT_CARD)
                              : state->players[params[0]].amounts[offset];

        arr[0] |= (not_neg && has_amount) << offset;
    }
    arr[1] = state->players->harbors;

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

    bool _is_bank = offset == MAX_PLAYERS;
    signed char *vec = bank;
    !_is_bank && (vec = (signed char *)manip(player));

    res = (unsigned char *)vector_dup(vec, (*size = count));

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

unsigned char *inf_player_materials(unsigned char *size, signed char *params,
                                    int socket, GameState state)
{
    unsigned char *mats = inf_players_manip(size,
                                            state->players,
                                            params[0],
                                            state->bankMaterials,
                                            TOTAL_MATERIALS,
                                            get_player_materials);
    return mats;
}
unsigned char *inf_player_devcards(unsigned char *size, signed char *params,
                                   int socket, GameState state)
{
    return inf_players_manip(size,
                             state->players,
                             params[0],
                             state->bankDevelopments,
                             TOTAL_DEVELOPMENT_CARD,
                             get_player_devcards);
}

unsigned char *inf_player_victory_points(unsigned char *size, signed char *params,
                                         int socket, GameState state)
{
    unsigned char victory_points = state->players[params[0]].victoryPoints;
    victory_points += state->achievementCards[BIGGEST_ARMY] == params[0];
    victory_points += state->achievementCards[LONGEST_PATH] == params[0];
    return single_byte(size, victory_points);
}

unsigned char *inf_player_amounts(unsigned char *size, signed char *params,
                                  int socket, GameState state)
{
    return (unsigned char *)vector_dup((signed char *)state->players[params[0]].amounts, (signed char)(*size = TOTAL_STORE - 1));
}
unsigned char *inf_achivement_cards(unsigned char *size, signed char *params,
                                    int socket, GameState state)
{
    return (unsigned char *)vector_dup(state->achievementCards, (*size = TOTAL_ACHIEVEMENTS_CARD));
}
unsigned char *single_byte(unsigned char *size, signed char value)
{
    unsigned char *res = malloc(sizeof(unsigned char) * (*size = 1));
    *res = value;
    return res;
}

void print_edge_offset(const void *ptr)
{
    EdgePtr edge = (EdgePtr)ptr;
    printt(" %d ", edge->offset);
}

// O(CE+CVE+C) | C is negligible
signed char update_longest_road(GraphPtr graph, signed char *longest_road_achievement)
{
    unsigned char color_offset, vertex_offset, max_road_color = BLACK;
    signed char temp_length, max_road_length = 0, max_road_per_color[MAX_PLAYERS] = {0}, old_val;

    for (color_offset = 0; color_offset < MAX_PLAYERS; color_offset++)
    {
        max_road_per_color[color_offset] = dfs_score(graph, color_offset);

        (max_road_per_color[color_offset] > max_road_length) &&
            (max_road_length = max_road_per_color[color_offset],
             max_road_color = color_offset);
    }

    // if the same max road length apply to only 1 player, only then change
    (vector_count(max_road_per_color,
                  MAX_PLAYERS,
                  max_road_length) == 1) &&
        (*longest_road_achievement = max_road_color);

    // if the max road length is below 5, return longest road player index to -1
    (max_road_length < 5) &&
        (*longest_road_achievement = -1);

    print_vec((unsigned char *)max_road_per_color, MAX_PLAYERS);

    return *longest_road_achievement;
}

unsigned char *switch_action_store(unsigned char *size, signed char *params,
                                   int socket, GameState state)
{
    unsigned char *res = calloc((*size = 1), sizeof(unsigned char));

    switch (params[0])
    {
    case 0:
        buy_road(state->players,
                 state->graph,
                 store[ROAD],
                 state->bankMaterials,
                 params[1],
                 params[2] + AREAS,
                 params[3] + AREAS);
        res[0] = update_longest_road(state->graph, state->achievementCards + LONGEST_PATH);
        break;
    case 1:
        res[0] = buy_settlement(state->players,
                                state->graph,
                                store[SETTLEMENT],
                                state->bankMaterials,
                                params[1],
                                params[2] + AREAS);
        break;
    case 2:
        res[0] = buy_city(state->players,
                          state->graph,
                          store[CITY],
                          state->bankMaterials,
                          params[2] + AREAS);
        break;
    case 3:
        res[0] = buy_developement(state->players,
                                  state->bankMaterials,
                                  state->bankDevelopments,
                                  store[DEVELOPMENT_CARD]);
        break;
    default:
        res[0] = false;
        break;
    }
    return res;
}

unsigned char *switch_dev_card(unsigned char *size, signed char *params,
                               int socket, GameState state)
{
    switch (params[0])
    {
    case 3:
    {
        signed char mats_to_transfer[TOTAL_MATERIALS] = {0};
        mats_to_transfer[params[1]]++;
        mats_to_transfer[params[2]]++;
        transfer_materials(state->players, state->bankMaterials, mats_to_transfer, true);
    }
    break;
    case 4:
        transfer_all_players_mats(state->players, 0, (signed char)state->num_of_players, params[1]);
    }
    state->players->developmentCards[params[0]]--;

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
        (extract_area_number(graph->vertices[offset].material_number) == rolled_num &&
         robberArea != offset) &&
            (vertecies[vertex++] = graph->vertices + offset);
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
        (players[num_of_players].knightUsed > players[maxIndex].knightUsed) &&
            (maxIndex = num_of_players);
    }

    *biggest_army_achievement = maxIndex;

    // if the knight used is below 3, then set the biggest army player index to -1
    (players[maxIndex].knightUsed < 3) &&
        (*biggest_army_achievement = -1);

    printt("\t\tmax = %d (%d)\n", players[maxIndex].knightUsed, maxIndex);
    return *biggest_army_achievement;
}
unsigned char *move_robber(unsigned char *size, signed char *params,
                           int socket, GameState state)
{
    unsigned char *res = single_byte(size, -1);
    unsigned char target_index = params[1]; // if got 7!
    (state->robberArea) = params[2];

    switch (target_index)
    {
    case 4:
        break;
    case 3:
    case 2:
    case 1:
    {
        unsigned char cost[TOTAL_MATERIALS] = {0};
        cost[random_index_by_vals(TOTAL_MATERIALS,
                                  (signed char *)state->players[target_index].materials)]++;
        transfer_materials(state->players + params[0],
                           (signed char *)state->players[target_index].materials,
                           (signed char *)cost, true);
        printt("steal mode player index: %d\n", target_index);
    }
    break;

    case 0:
        state->players[params[0]].knightUsed++;
        printt("pullin up knight!! %d\n", state->players[params[0]].knightUsed);
        break;
    }

    res[0] = update_biggest_army(state->players, state->num_of_players,
                                 state->achievementCards + BIGGEST_ARMY);
    printt("returning %hhd\n", *res);
    return res;
}
unsigned char *drop_materials(unsigned char *size, signed char *params,
                              int socket, GameState state)
{
    transfer_materials(state->players + params[0], state->bankMaterials, params + 2, false);

    unsigned char offset;
    for (offset = 1; offset < state->num_of_players && params[1]; offset++)
    {
        vector_sum((signed char *)state->players[offset].materials, TOTAL_MATERIALS) >= 7 && bot_drop_materials(state->players + offset, state);
    }

    return single_byte(size, 0);
}

unsigned char *make_a_deal(unsigned char *size, signed char *params, int socket, GameState state)
{

    unsigned char deal_num = params[0];
    unsigned char deal_mat_from = (deal_num & 0x07) - 1;
    unsigned char deal_mat_to = (deal_num >> 3) - 1;

    bool conditions[3] = {state->players->harbors >> deal_mat_from, state->players->harbors >> TOTAL_MATERIALS, true};

    unsigned char deal_type = find_first_true_index(conditions, 3);
    printt("local player deal: from: %d to: %d type: %d\n", deal_mat_from, deal_mat_to, deal_type);

    handle_deal(state->players, state, deal_mat_from, deal_mat_to, deal_type);

    return single_byte(size, 0);
}

// bots
bool bot_drop_materials(PlayerPtr player, GameState state)
{
    unsigned char astIndex = state->astIndexes[player->color - 1];
    unsigned char amount = vector_sum((signed char *)player->materials, TOTAL_MATERIALS) / 2;

    unsigned char *(*func[3])(GraphPtr, PlayerPtr) = {woodMatsOrder,
                                                      wheatMatsOrder,
                                                      cardsMatsOrder};

    unsigned char *res = func[astIndex](state->graph, player);
    unsigned char *order = (unsigned char *)vector_dup((signed char *)res + TOTAL_MATERIALS, TOTAL_MATERIALS);

    free(res);

    while (amount--)
    {
        unsigned char cost[TOTAL_MATERIALS] = {0};
        signed char index = vector_order_find_last((signed char *)player->materials, (signed char *)order, TOTAL_MATERIALS, above_zero);
        cost[index]++;
        transfer_materials(player, state->bankMaterials, (signed char *)cost, false);
    }

    free(order);

    return true;
}
bool bot_plays(PlayerPtr player, int socket, GameState state)
{
    unsigned char astIndex = state->astIndexes[player->color - 1];
    unsigned char size, *buffer, *temp;
    bool need_stealing;
    usleep(200000);

    // open round by rolling the dice
    buffer = calloc(2, sizeof(unsigned char));
    temp = roll_dice(&size, NULL, socket, state);
    vector_cpy((signed char *)(buffer + 1), (signed char *)temp, size);

    need_stealing = (temp[0] & 0x0F) + (temp[0] >> 4) == 7;

    free(temp);

    buffer[0] = 3;

    size = 2;

    print_vec(buffer, 2);

    BOT_SEND_FREE(socket, size, buffer);

    (need_stealing) && (usleep(200000), state_steal(player, socket, state));

    bool (**conditionsFunctions)(PlayerPtr, GameState, QueuePtr);
    void (**actionsFunctions)(PlayerPtr, int, GameState, QueuePtr);

    void (*init_actions[])(bool (***)(PlayerPtr, GameState, QueuePtr), void (***)(PlayerPtr, int, GameState, QueuePtr)) = {
        wood_init_actions,
        wheat_init_actions,
        cards_init_actions};

    init_actions[astIndex](&conditionsFunctions, &actionsFunctions);
    putts("after init");
    usleep(200000);

    play_actions_with_conditions(state,
                                 player,
                                 socket,
                                 conditionsFunctions,
                                 actionsFunctions, 4);
    putts("after play");

    free(conditionsFunctions);
    free(actionsFunctions);

    usleep(400000);
    return need_stealing;
}

bool bot_buy_first(PlayerPtr player, int socket, GameState state)
{

    putts("bot_buy_first");
    usleep(200000);

    unsigned char size;
    unsigned char astIndex = state->astIndexes[player->color - 1];
    Heap *heap = state->astHeaps + astIndex;

    unsigned short (*prioritiesRoad[2])(GraphPtr, PlayerPtr, Heap *, StackPtr) = {prioritiseWoodRoad, prioritiseWheatCardsRoad};

    unsigned char vertex = 0;
    putts("start searching vertex");

    vertex = convert_void_ptr_to_unsigned_char(heap_top(heap));

    while (!(state->graph->vertices[vertex].color == BLACK && buyableSettlement(player, state->graph->vertices + vertex, false)))
    {
        putts("\t kicking vertex");

        heap_extract(heap, heap_max, NULL);
        vertex = convert_void_ptr_to_unsigned_char(heap_top(heap));
    }

    heap_extract(heap, heap_max, NULL);
    putts("buy settlement");

    buy_settlement(player, state->graph, store[SETTLEMENT],
                   state->bankMaterials, -state->turnOffset / state->num_of_players, vertex);

    unsigned char *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 1;
    buffer[1] = vertex;

    BOT_SEND_FREE(socket, size, buffer);
    usleep(200000);

    putts("prefered road");

    Stack buyableRoads;
    stack_init(&buyableRoads);

    buyableRoadsAroundVertex(state->graph, &buyableRoads, player, vertex);
    putts("after buyable roads");

    unsigned short prefered_road = prioritiesRoad[!!astIndex](state->graph, player, heap, &buyableRoads);
    printt("maybe road %d to %d\n", prefered_road & 0xFF, prefered_road >> 8);

    printt("buying road %d to %d\n", prefered_road & 0xFF, prefered_road >> 8);

    buy_road(player, state->graph, store[ROAD], state->bankMaterials, false, prefered_road & 0xFF, prefered_road >> 8);
    putts("calloc");

    buffer = calloc(size = 3, sizeof(unsigned char));
    buffer[0] = 2;
    buffer[1] = prefered_road & 0xFF;
    buffer[2] = prefered_road >> 8;

    putts("sending");

    BOT_SEND_FREE(socket, size, buffer);

    usleep(200000);
    return false;
}

unsigned char *handle_rest_turns(unsigned char *size, signed char *params,
                                 int socket, GameState state)
{
    (state->turnOffset)++;
    unsigned char turnColor, _size = 3;
    unsigned char _buff[3] = {0};
    bool (*playFn[2])(PlayerPtr, int, GameState) = {bot_buy_first, bot_plays};
    bool initial_state = state->turnOffset / state->num_of_players > 1;
    bool stealing;

    // while the turn color its not WHITE
    while ((turnColor = state->turnOffset % state->num_of_players))
    {
        printt("start: player index %d", turnColor);

        stealing = playFn[initial_state](state->players + turnColor, socket, state);
        putts("player done");

        (state->turnOffset)++;

        _buff[0] = 0;
        _buff[1] = (state->turnOffset) % state->num_of_players;
        _buff[2] = stealing && vector_sum((signed char *)state->players->materials, TOTAL_MATERIALS) >= 7;

        BOT_SEND(socket, _size, _buff)
        // printt("\tbot number %d done playing!", turnColor);
    }

    putts("done");

    *size = 0;
    return NULL;
}
unsigned char *close_server(unsigned char *size, signed char *params, int socket, GameState state)
{
    exit(EXIT_SUCCESS);
}
