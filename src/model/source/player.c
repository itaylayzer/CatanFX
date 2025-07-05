#include "../headers/player.h"

// helpers

float *materials_probabilities(GraphPtr graph, unsigned char vertex)
{
    float *mats_prob = calloc(TOTAL_MATERIALS, sizeof(float));
    Node node;
    EdgePtr edge;
    VertexPtr tempVertex;
    bool condition;

    QUEUE_TRAVARSE(graph->vertices[vertex].edges, node);

    edge = node->data;
    condition = (edge->color == GRAY);

    tempVertex = edge->vertex;

    mats_prob[extract_area_materials(tempVertex->material_number)] +=
        probs[(tempVertex->material_number >> 3) - 2] * condition;

    QUEUE_TRAVARSE_FINISH;

    return mats_prob;
}

Stack areaSettlements(VertexPtr vertex)
{
    Stack stk;

    Node node;
    VertexPtr dest;
    EdgePtr edge;

    stack_init(&stk);

    QUEUE_TRAVARSE(vertex->edges, node);
    edge = node->data;
    dest = edge->vertex;
    (edge->color == GRAY) &&
        (stack_push(&stk, dest));

    QUEUE_TRAVARSE_FINISH;

    return stk;
}
bool buyableSettlement(PlayerPtr player, VertexPtr source, bool needRoad)
{
    bool dontHaveHouseNear = true;
    bool connectedByRoad = !needRoad;

    Node node;
    EdgePtr edge;
    VertexPtr dest;

    QUEUE_TRAVARSE(source->edges, node);

    edge = node->data;
    dest = edge->vertex;

    // is near vertecies are bought already
    ((dest->color & 0x0F) < BLACK) &&
        (dontHaveHouseNear = false);

    // if has a near road
    (needRoad && edge->color == player->color) &&
        (connectedByRoad = true);

    QUEUE_TRAVARSE_FINISH;

    return connectedByRoad && dontHaveHouseNear;
}
void initScoreHeaps(GraphPtr graph, Heap heaps[TOTAL_ASTRATEGIES])
{
    Heap *astWood = heaps;
    Heap *astWheat = heaps + 1;
    Heap *astCards = heaps + 2;

    heap_init(astWood, VERTECIES);
    heap_init(astWheat, VERTECIES);
    heap_init(astCards, VERTECIES);

    unsigned char settlement_offset;
    float *mats_prob;

    for (settlement_offset = AREAS;
         settlement_offset < VERTECIES + AREAS;
         settlement_offset++)
    {

        mats_prob = materials_probabilities(graph, settlement_offset);

        heap_insert(astWood, convert_unsigned_char_to_void_ptr(settlement_offset),
                    woodScore(mats_prob), heap_max);
        heap_insert(astWheat, convert_unsigned_char_to_void_ptr(settlement_offset),
                    wheatScore(mats_prob), heap_max);
        heap_insert(astCards, convert_unsigned_char_to_void_ptr(settlement_offset),
                    cardsScore(mats_prob), heap_max);

        free(mats_prob);
    }
}
unsigned char getMissingMaterial(signed char playerMaterials[TOTAL_MATERIALS],
                                 const signed char productMats[TOTAL_MATERIALS])
{
    signed char *sub = vector_sub(playerMaterials, productMats, TOTAL_MATERIALS);
    const unsigned char min_index = vector_min_index(sub, TOTAL_MATERIALS);

    free(sub);
    return min_index;
}

unsigned char *leastImportent(unsigned char astrategy, PlayerPtr player, GraphPtr graph)
{
    unsigned char *(*func[3])(GraphPtr, PlayerPtr) = {woodMatsOrder,
                                                      wheatMatsOrder,
                                                      cardsMatsOrder};

    unsigned char *res = func[astrategy](graph, player);
    unsigned char *mats = (unsigned char *)vector_dup((signed char *)res + TOTAL_MATERIALS, TOTAL_MATERIALS);

    free(res);

    vector_reverse((signed char *)mats, TOTAL_MATERIALS);

    return mats;
}

unsigned char buyableMaterial(unsigned char astrategy,
                              PlayerPtr player,
                              GraphPtr graph,
                              unsigned char missingMaterial,
                              signed char playerMaterials[TOTAL_MATERIALS],
                              unsigned char playerHarbors)
{
    unsigned char harborOffset, harbor, take, result = 0;
    unsigned char *least_importent = leastImportent(astrategy, player, graph);
    bool condition;

    for (harborOffset = 0; harborOffset < TOTAL_MATERIALS && !result; harborOffset++)
    {
        harbor = least_importent[harborOffset];
        condition = (playerHarbors >> harborOffset & 0x01 &&
                     vector_all(playerMaterials, TOTAL_MATERIALS, above_equal_zero));

        playerMaterials[harborOffset] -= condition * 2;
        playerMaterials[missingMaterial] += condition;

        (condition) &&
            (result = harborOffset + 1 + (missingMaterial >> 5));

        take = 4;
        (playerHarbors >> 5) &&
            (take = 3);

        condition = (playerMaterials[harbor] - take >= 0 && !result);

        playerMaterials[harborOffset] -= take * condition;
        playerMaterials[missingMaterial] += condition;

        (condition) &&
            (result = harborOffset + 1 + ((take - 2) >> 3) + (missingMaterial >> 5));
    }

    return result;
}

// astrategies scores
float woodScore(float mats[TOTAL_MATERIALS])
{
    return mats[WOOD] + mats[BRICK];
}
float wheatScore(float mats[TOTAL_MATERIALS])
{
    return mats[WHEAT] + mats[BRICK];
}
float cardsScore(float mats[TOTAL_MATERIALS])
{
    return mats[WHEAT] + mats[BRICK] + mats[ORE];
}

// picking astrategies
unsigned char *astrategies_init(GraphPtr graph, Heap heaps[TOTAL_ASTRATEGIES])
{
    initScoreHeaps(graph, heaps);

    unsigned char offset, min_index;
    signed char vertex_score, scores[TOTAL_ASTRATEGIES] = {0};

    void *vertex;
    unsigned char *indexes = calloc(TOTAL_ASTRATEGIES, sizeof(unsigned char));

    for (offset = 0; offset < TOTAL_ASTRATEGIES; offset++)
    {
        vertex = heap_extract(heaps + offset, heap_max, &vertex_score);
        scores[offset] = vertex_score;
        scores[offset] += heap_top_score(heaps + offset);

        heap_insert(heaps + offset, vertex, vertex_score, heap_max);
    }

    offset = TOTAL_ASTRATEGIES;
    min_index = vector_min_index(scores, TOTAL_ASTRATEGIES);

    while (scores[min_index] != SIGNED_MAX_VALUE &&
           --offset >= 0)
    {
        indexes[offset] = min_index;
        scores[min_index] = SIGNED_MAX_VALUE;

        min_index = vector_min_index(scores, TOTAL_ASTRATEGIES);
    }
    print_vec(indexes, 3);
    return indexes;
}

unsigned short prioritiseWoodRoad(GraphPtr graph,
                                  PlayerPtr player,
                                  Heap heaps[TOTAL_ASTRATEGIES],
                                  StackPtr buyable_roads)
{
    putts("prioritiseWoodRoad");
    Heap bestWheatScores, wheatScores, roadScores;

    EdgePtr tempEdge;
    VertexPtr tempVertex;

    // putts("befor init to heaps");
    heap_init(&bestWheatScores, TOTAL_ROADS * 2);
    heap_init(&wheatScores, TOTAL_ROADS * 2);
    heap_init(&roadScores, TOTAL_ROADS * 2);
    putts("after heap_init");

    unsigned char offset, vertex, currentRoadScore, roadScore, bestWheatScore, best_vertex = convert_void_ptr_to_unsigned_char(heap_top(heaps + AST_WHEAT));
    signed char best_vertex_score = heap_top_score(heaps + AST_WHEAT);
    float *mats_prob;
    unsigned short edge_num;
    putts("after heap_init");

    while (!stack_empty(*buyable_roads))
    {
        putts("\tprioritiseWoodRoad");

        edge_num = convert_void_ptr_to_unsigned_short(stack_pop(buyable_roads));
        vertex = edge_num >> 8;

        mats_prob = materials_probabilities(graph, vertex);

        printt("\t\tF [%f %f %f %f %f] \n", mats_prob[0], mats_prob[1], mats_prob[2], mats_prob[3], mats_prob[4]);

        putts("\t\tbefore djkstra");
        graph_dijkstra(graph, vertex, BLACK);
        putts("\t\tafter dijkstra");
        bestWheatScore = best_vertex_score / graph_dijkstra_distance(graph, best_vertex);

        currentRoadScore = dfs_score(graph, player->color);

        // apply purchasing
        change_road(player->color, graph, edge_num & 0xFF, vertex);
        putts("\t\tafter change_road");

        roadScore = (dfs_score(graph, player->color) - currentRoadScore) * 50;

        // cancel purchasing
        change_road(BLACK, graph, edge_num & 0xFF, vertex);
        putts("\t\tafter change_road");
        heap_insert(&wheatScores, convert_unsigned_short_to_void_ptr(edge_num), wheatScore(mats_prob), heap_max);
        heap_insert(&bestWheatScores, convert_unsigned_short_to_void_ptr(edge_num), bestWheatScore, heap_max);
        heap_insert(&roadScores, convert_unsigned_short_to_void_ptr(edge_num), roadScore * 50, heap_max);

        free(mats_prob);
    }

    edge_num = convert_void_ptr_to_unsigned_short(heap_top(&bestWheatScores));
    unsigned char tempEdgeScore, bestEdgeScore = heap_top_score(&bestWheatScores);

    Heap *tempHeap, *ptrHeaps[2] = {&wheatScores, &roadScores};

    for (offset = 0; offset < 2; offset++)
    {
        tempHeap = ptrHeaps[offset];
        tempEdgeScore = heap_top_score(tempHeap);

        (tempEdgeScore > bestEdgeScore) &&
            (bestEdgeScore = tempEdgeScore, edge_num = convert_void_ptr_to_unsigned_short(heap_top(tempHeap)));
    }

    heap_destroy(&bestWheatScores);
    heap_destroy(&wheatScores);
    heap_destroy(&roadScores);
    putts("after prioritiseWoodRoad");
    return edge_num;
}
unsigned short prioritiseWheatCardsRoad(GraphPtr graph,
                                        PlayerPtr player,
                                        Heap heaps[TOTAL_ASTRATEGIES],
                                        StackPtr buyable_roads)
{

    putts("prioritiseWheatCardsRoad");
    // putts("starting");

    EdgePtr tempEdge;
    VertexPtr tempVertex;
    // putts("before top");

    unsigned char vertex, bestWoodScore, best_vertex = convert_void_ptr_to_unsigned_char(heap_top(heaps + AST_WOOD));
    putts("after heap_top");

    signed char best_vertex_score = heap_top_score(heaps + AST_WOOD);
    putts("after heap_top_score");

    // putts("after top");

    unsigned short edge_num, temp_edge_num;
    signed char heapScore, bestScore;

    float *mats_prob;

    Heap bestWoodScores, woodScores;
    // putts("befor init to heaps");
    heap_init(&bestWoodScores, TOTAL_ROADS * 2);
    heap_init(&woodScores, TOTAL_ROADS * 2);

    while (!stack_empty(*buyable_roads))
    {

        putts("loop");

        edge_num = convert_void_ptr_to_unsigned_short(stack_pop(buyable_roads));
        vertex = edge_num >> 8;
        printt("after stack_pop edge_num=%d vertex=%d\n", edge_num, vertex);

        mats_prob = materials_probabilities(graph, vertex);
        putts("after materials_probabilities");

        printt("F [%f %f %f %f %f] \n", mats_prob[0], mats_prob[1], mats_prob[2], mats_prob[3], mats_prob[4]);

        putts("before djkstra");
        graph_dijkstra(graph, vertex, BLACK);
        putts("after dijkstra");

        unsigned char dist = graph_dijkstra_distance(graph, best_vertex);

        if (dist == 0)
        {
            putts("Error: graph_dijkstra_distance returned 0 — cannot divide");
            bestWoodScore = 0;
        }
        else
        {
            bestWoodScore = best_vertex_score / dist;
        }

        heap_insert(&woodScores, convert_unsigned_short_to_void_ptr(edge_num), woodScore(mats_prob), heap_max);
        heap_insert(&bestWoodScores, convert_unsigned_short_to_void_ptr(edge_num), bestWoodScore, heap_max);

        free(mats_prob);
    }
    putts("\nafter main loop\nextract1");
    edge_num = convert_void_ptr_to_unsigned_short(heap_extract(&woodScores, heap_max, &heapScore));
    putts("extract2");

    temp_edge_num = convert_void_ptr_to_unsigned_short(heap_extract(&bestWoodScores, heap_max, &bestScore));

    (bestScore > heapScore) &&
        (edge_num = temp_edge_num);

    heap_destroy(&bestWoodScores);
    heap_destroy(&woodScores);

    // printt("\t\tedge_num:%d->%d (heapScore:%d), temp_edge_num:%d->%d (bestScore:%d)", edge_num & 0xFF, edge_num >> 8, heapScore, temp_edge_num & 0xFF, temp_edge_num >> 8, bestScore);

    return edge_num;
}
unsigned char prioritiseWheatCardsSettlement(GraphPtr graph,
                                             PlayerPtr player,
                                             Heap heaps[TOTAL_ASTRATEGIES])
{
    EdgePtr edge;
    VertexPtr tempVertex;
    unsigned char vertex;

    float *mats_prob;

    Heap scores;
    heap_init(&scores, VERTECIES);

    Stack stk;
    stack_init(&stk);

    buyableSettlements(&stk, player, graph, true);

    while (!stack_empty(stk))
    {
        vertex = convert_void_ptr_to_unsigned_char(stack_pop(&stk));
        mats_prob = materials_probabilities(graph, vertex);

        heap_insert(&scores, convert_unsigned_char_to_void_ptr(vertex), woodScore(mats_prob), heap_max);

        free(mats_prob);
    }

    signed char score = 0;
    vertex = convert_void_ptr_to_unsigned_char(heap_extract(&scores, heap_max, &score));

    heap_destroy(&scores);

    return vertex;
}

unsigned char nearest_distance(GraphPtr graph, PlayerPtr player, unsigned char vertex)
{
    Node node;
    unsigned char distance = MAX_VALUE;
    QUEUE_TRAVARSE(player->settlements, node);

    graph_dijkstra(graph, convert_void_ptr_to_unsigned_char(node->data), BLACK);
    distance = bmin(distance, graph_dijkstra_distance(graph, vertex));

    QUEUE_TRAVARSE_FINISH;

    return distance;
}

unsigned char prioritiseWoodSettlement(GraphPtr graph, PlayerPtr player, Heap heaps[TOTAL_ASTRATEGIES])
{
    unsigned char vertex, vertexScore, best_vertex;

    float *mats_prob;
    Heap wheatScores, vertexScores;
    Stack stk;

    heap_init(&wheatScores, VERTECIES);
    heap_init(&vertexScores, VERTECIES);

    buyableRoads(graph, &stk, player);

    while (!stack_empty(stk))
    {
        vertex = convert_void_ptr_to_unsigned_char(stack_pop(&stk));

        mats_prob = materials_probabilities(graph, vertex);
        vertexScore = 70 / nearest_distance(graph, player, vertex);

        heap_insert(&wheatScores, convert_unsigned_char_to_void_ptr(vertex), wheatScore(mats_prob), heap_max);

        heap_insert(&vertexScores, convert_unsigned_char_to_void_ptr(vertex), vertexScore, heap_max);

        free(mats_prob);
    }

    best_vertex = convert_void_ptr_to_unsigned_char(heap_top(&wheatScores));

    (heap_top_score(&vertexScores) > heap_top_score(&wheatScores)) &&
        (best_vertex = convert_void_ptr_to_unsigned_char(heap_top(&vertexScores)));

    heap_destroy(&wheatScores);
    heap_destroy(&vertexScores);

    return best_vertex;
}

// astrategies prioritise functions
unsigned char *woodMatsOrder(GraphPtr graph, PlayerPtr player)
{
    Stack temp;
    stack_init(&temp);

    bool states[TOTAL_STORE] =
        {player->amounts[ROAD] && buyableRoads(graph, &temp, player),
         player->amounts[SETTLEMENT] && buyableSettlements(&temp, player, graph, true),
         player->amounts[CITY] && upgradeableSettlements(&temp, player, graph),
         true};

    unsigned char values[TOTAL_STORE][TOTAL_MATERIALS * 2] =
        {{store[ROAD][0], store[ROAD][1], store[ROAD][2], store[ROAD][3], store[ROAD][4], BRICK, WOOD, ORE, WHEAT, WOOL},

         {store[SETTLEMENT][0], store[SETTLEMENT][1], store[SETTLEMENT][2], store[SETTLEMENT][3], store[SETTLEMENT][4], BRICK, WOOD, WHEAT, WOOL, ORE},

         {store[CITY][0], store[CITY][1], store[CITY][2], store[CITY][3], store[CITY][4], ORE, WHEAT, BRICK, WOOD, WOOL},

         {store[DEVELOPMENT_CARD][0], store[DEVELOPMENT_CARD][1], store[DEVELOPMENT_CARD][2], store[DEVELOPMENT_CARD][3], store[DEVELOPMENT_CARD][4], ORE, WHEAT, WOOL, BRICK, WOOD}};

    unsigned char index = find_first_true_index(states, TOTAL_STORE);
    unsigned char *res = (unsigned char *)vector_dup((signed char *)values[index], TOTAL_MATERIALS * 2);
    stack_destroy(&temp);

    return res;
}
unsigned char *wheatMatsOrder(GraphPtr graph, PlayerPtr player)
{
    Stack temp;
    stack_init(&temp);

    bool states[TOTAL_STORE - 1] =
        {player->amounts[CITY] && upgradeableSettlements(&temp, player, graph), player->amounts[SETTLEMENT] && buyableSettlements(&temp, player, graph, true),
         true};

    unsigned char values[TOTAL_STORE - 1][TOTAL_MATERIALS * 2] =
        {{store[CITY][0], store[CITY][1], store[CITY][2], store[CITY][3], store[CITY][4], ORE, WHEAT, BRICK, WOOD, WOOL},

         {store[SETTLEMENT][0], store[SETTLEMENT][1], store[SETTLEMENT][2], store[SETTLEMENT][3], store[SETTLEMENT][4], BRICK, WOOD, WHEAT, WOOL, ORE},

         {store[DEVELOPMENT_CARD][0], store[DEVELOPMENT_CARD][1], store[DEVELOPMENT_CARD][2], store[DEVELOPMENT_CARD][3], store[DEVELOPMENT_CARD][4], ORE, WHEAT, WOOL, BRICK, WOOD}};

    unsigned char index = find_first_true_index(states, TOTAL_STORE - 1);
    unsigned char *res = (unsigned char *)vector_dup((signed char *)values[index], TOTAL_MATERIALS * 2);
    stack_destroy(&temp);

    return res;
}
unsigned char *cardsMatsOrder(GraphPtr graph, PlayerPtr player)
{
    unsigned char value[TOTAL_MATERIALS * 2] = {store[DEVELOPMENT_CARD][0],
                                                store[DEVELOPMENT_CARD][1],
                                                store[DEVELOPMENT_CARD][2],
                                                store[DEVELOPMENT_CARD][3],
                                                store[DEVELOPMENT_CARD][4],
                                                ORE < WHEAT, WOOL, BRICK, WOOD};
    return (unsigned char *)vector_dup((signed char *)value, TOTAL_MATERIALS * 2);
}

// astrategies play function
void wood_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                       void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr))
{
    *ptr_conditions = malloc(4 * sizeof(bool (*)(PlayerPtr, GameState, QueuePtr)));
    *ptr_actions = malloc(4 * sizeof(void (*)(PlayerPtr, int, GameState, QueuePtr)));

    (*ptr_conditions)[0] = state_can_buy_settlement;
    (*ptr_actions)[0] = state_buy_settlement;

    (*ptr_conditions)[1] = state_can_buy_road;
    (*ptr_actions)[1] = state_buy_road;

    (*ptr_conditions)[2] = state_can_buy_city;
    (*ptr_actions)[2] = state_buy_city;

    (*ptr_conditions)[3] = state_can_buy_development;
    (*ptr_actions)[3] = state_buy_development;
}
void wheat_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                        void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr))
{
    *ptr_conditions = malloc(4 * sizeof(bool (*)(PlayerPtr, GameState, QueuePtr)));
    *ptr_actions = malloc(4 * sizeof(void (*)(PlayerPtr, int, GameState, QueuePtr)));

    (*ptr_conditions)[0] = state_can_buy_city;
    (*ptr_actions)[0] = state_buy_city;

    (*ptr_conditions)[1] = state_can_buy_development;
    (*ptr_actions)[1] = state_buy_development;

    (*ptr_conditions)[2] = state_can_buy_settlement;
    (*ptr_actions)[2] = state_buy_settlement;

    (*ptr_conditions)[3] = state_can_buy_road;
    (*ptr_actions)[3] = state_buy_road;
}
void cards_init_actions(bool (***ptr_conditions)(PlayerPtr, GameState, QueuePtr),
                        void (***ptr_actions)(PlayerPtr, int, GameState, QueuePtr))
{
    *ptr_conditions = malloc(4 * sizeof(bool (*)(PlayerPtr, GameState, QueuePtr)));
    *ptr_actions = malloc(4 * sizeof(void (*)(PlayerPtr, int, GameState, QueuePtr)));

    (*ptr_conditions)[0] = state_can_buy_development;
    (*ptr_actions)[0] = state_buy_development;

    (*ptr_conditions)[1] = state_can_buy_city;
    (*ptr_actions)[1] = state_buy_city;

    (*ptr_conditions)[2] = state_can_buy_settlement;
    (*ptr_actions)[2] = state_buy_settlement;

    (*ptr_conditions)[3] = state_can_buy_road;
    (*ptr_actions)[3] = state_buy_road;
}
void play_actions_with_conditions(GameState state,
                                  PlayerPtr player,
                                  int socket,
                                  bool (**conditions)(PlayerPtr, GameState, QueuePtr),
                                  void (**actions)(PlayerPtr, int, GameState, QueuePtr),
                                  unsigned char size)
{
    unsigned char offset;
    for (offset = 0; offset < size; offset++)
    {
        printt("offset=%d\n", offset);

        Queue queue;
        queue_init(&queue);
        while (conditions[offset](player, state, &queue))
        {
            putts("condition worked");
            actions[offset](player, socket, state, &queue);

            queue_destroy(&queue);
            queue_init(&queue);

            usleep(200000);
        }

        queue_destroy(&queue);
        usleep(200000);
    }
}

// generic functions
unsigned char prioritiseUpgradeableSettlement(PlayerPtr player, GraphPtr graph,
                                              float (*scoreFn)(float[TOTAL_MATERIALS]))
{
    Node node;
    EdgePtr edge;
    VertexPtr vertex;
    Heap scores;
    heap_init(&scores, VERTECIES);

    float *mats_probs;
    unsigned char mat_number, vertex_index;

    QUEUE_TRAVARSE(player->settlements, node);

    vertex_index = convert_void_ptr_to_unsigned_char(node->data);
    mats_probs = materials_probabilities(graph, vertex_index);

    heap_insert(&scores, convert_unsigned_char_to_void_ptr(vertex_index),
                scoreFn(mats_probs), heap_max);

    free(mats_probs);

    QUEUE_TRAVARSE_FINISH;

    const void *vertexOffset = heap_extract(&scores, heap_max, NULL);

    heap_destroy(&scores);

    return convert_void_ptr_to_unsigned_char(vertexOffset);
}
unsigned char moveRobberTo(PlayerPtr player, GraphPtr graph, unsigned char current_robber)
{
    Heap scores;
    Stack settlements;
    VertexPtr vertex, settlement;
    unsigned char offset, score;
    float prob;
    bool belongToPlayer;
    ;
    heap_init(&scores, AREAS);

    for (offset = 0; offset < AREAS; offset++)
    {
        vertex = graph->vertices + offset;
        score = 0;
        settlements = areaSettlements(vertex);
        prob = probs[vertex->material_number >> 3];
        belongToPlayer = false;

        while (!stack_empty(settlements))
        {
            settlement = stack_pop(&settlements);
            belongToPlayer |= (settlement->color & 0x0F) == player->color;

            (settlement->color < BLACK) &&
                (score += prob);
        }

        // we want to move to another place then the current place!
        score *= !!(offset - current_robber);

        (belongToPlayer == false) &&
            heap_insert(&scores, convert_unsigned_char_to_void_ptr(offset), score, heap_max);
    }

    const void *vertexOffset = heap_extract(&scores, heap_max, NULL);
    return convert_void_ptr_to_unsigned_char(vertexOffset);
}
unsigned char buyableRoadsAroundVertex(GraphPtr graph, StackPtr stk, PlayerPtr player, unsigned char vertex)
{
    Node node;
    VertexPtr tempVertex;
    EdgePtr tempEdge;
    bool condition;
    unsigned char size = 0;

    unsigned short edge_num;

    QUEUE_TRAVARSE(graph->vertices[vertex].edges, node);

    tempEdge = node->data;
    tempVertex = tempEdge->vertex;
    condition = tempVertex->color == BLACK;
    (condition) &&
        (stack_push(stk,
                    convert_unsigned_short_to_void_ptr(
                        (edge_num = vertex + (tempEdge->offset << 8)))));
    size += condition;

    QUEUE_TRAVARSE_FINISH;

    return size;
}
unsigned char buyableRoads(GraphPtr graph, StackPtr stk, PlayerPtr player)
{

    unsigned char count = 0, from, *roadArray, offset;
    unsigned short edge_num;
    Queue sourceQ, destQ;
    Node node;
    VertexPtr dest;
    EdgePtr dest_edge;
    bool condition;

    queue_init(&sourceQ);
    queue_init(&destQ);

    enqueue(&sourceQ, player->roads);

    QUEUE_TRAVARSE_BODY(sourceQ, node);
    roadArray = ((unsigned char *)&node->data);

    for (offset = 0; offset < 2; offset++)
    {
        from = roadArray[offset];
        dest = graph->vertices + from;
        enqueue(&destQ, dest->edges);

        QUEUE_TRAVARSE_BODY(destQ, node);

        dest_edge = node->data;
        edge_num = from + (dest_edge->offset << 8);

        condition = dest_edge->color == BLACK;
        count += condition;

        condition &&
            (stack_push(stk, convert_unsigned_short_to_void_ptr(edge_num)));

        QUEUE_TRAVARSE_FINISH;
    }
    QUEUE_TRAVARSE_FINISH;
    return count;
}
unsigned char buyableSettlements(StackPtr stk, PlayerPtr player, GraphPtr graph, bool needRoad)
{
    unsigned char count = 0;
    bool condition;
    unsigned char offset;
    VertexPtr vertex;

    for (offset = 0; offset < VERTECIES; offset++)
    {
        vertex = graph->vertices + offset + AREAS;
        condition = vertex->color == BLACK && buyableSettlement(player, vertex, needRoad);
        count += condition;
        condition &&stack_push(stk, vertex);
    }

    return count;
}
unsigned char upgradeableSettlements(StackPtr stk, PlayerPtr player, GraphPtr graph)
{
    unsigned char vertex, count = 0;

    Node node;

    QUEUE_TRAVARSE(player->settlements, node);

    vertex = (unsigned char)node->data;

    // if settlement vertex and dont have house
    (vertex >= AREAS && graph->vertices[vertex].color >> 6 == 0) &&
        stack_push(stk, convert_unsigned_char_to_void_ptr(vertex));

    QUEUE_TRAVARSE_FINISH;

    return count;
}

signed char *calc_deals_mats(unsigned char playerHarbors)
{
    unsigned char offset;
    signed char *mats = calloc(TOTAL_MATERIALS, sizeof(signed char));

    for (offset = 0; offset < TOTAL_MATERIALS; offset++)
    {
        bool conditions[3] = {(playerHarbors >> offset) & 0x01, (playerHarbors >> 5) & 0x01, true};
        mats[offset] = find_first_true_index(conditions, 3) + 2;
    }
    return mats;
}

bool buyableProduct(unsigned char astrategy,
                    PlayerPtr player,
                    GraphPtr graph,
                    signed char playerMats[TOTAL_MATERIALS],
                    signed char playerHarbors,
                    const signed char productMats[TOTAL_MATERIALS],
                    QueuePtr actionsQ)
{

    signed char *clone, *sub, *dealsMats;
    unsigned char res, missingMat;
    bool buyable;

    clone = vector_dup(playerMats, TOTAL_MATERIALS);
    dealsMats = calc_deals_mats(playerHarbors);
    res = 0;

    putts("dealsMats ");
    print_vec((unsigned char *)dealsMats, TOTAL_MATERIALS);
    while (
        !(buyable = vector_manip_condition(clone, productMats, TOTAL_MATERIALS, vector_sub, above_equal_zero) & 0x01) && (vector_manip_condition(clone, dealsMats, TOTAL_MATERIALS, vector_sub, above_equal_zero) >> 1) &&
        (res = buyableMaterial(astrategy, player, graph, getMissingMaterial(clone, productMats), clone, playerHarbors)) && vector_all(clone, TOTAL_MATERIALS, above_equal_zero))
    {
        printt("\tres:(to:%d, deal:%d, from:%d) |clone:", res & 0x07, (res >> 3) & 0x03, res >> 5);
        // printt("\tclone:");
        // times++;

        print_vec((unsigned char *)clone, TOTAL_MATERIALS);

        enqueue(actionsQ, convert_unsigned_char_to_void_ptr(res));
    }

    res &&
        enqueue(actionsQ, convert_unsigned_char_to_void_ptr(res));

    free(dealsMats);
    printt("buyable=%d\n", buyable);
    return buyable;
}

// condition funcitons

bool state_can_buy_road(PlayerPtr player, GameState state, QueuePtr queue)
{
    putts("state_can_buy_road");

    Stack stk;
    stack_init(&stk);

    unsigned char ast = state->astIndexes[player->color - 1];

    bool can_buy =
        // if buyable by player materials and bank/harbor deals
        buyableProduct(ast, player, state->graph,
                       (signed char *)player->materials, player->harbors, store[ROAD], queue)
        // if player have amount
        && player->amounts[ROAD]
        // if player have buyable roads
        && buyableRoads(state->graph, &stk, player)
        // if bank have materials
        && (vector_manip_condition(state->bankMaterials, store[ROAD], TOTAL_MATERIALS, vector_sub, above_equal_zero) & 0x01);

    stack_destroy(&stk);

    return can_buy;
}
bool state_can_buy_settlement(PlayerPtr player, GameState state, QueuePtr queue)
{
    putts("state_can_buy_settlement");

    Stack stk;
    stack_init(&stk);

    unsigned char ast = state->astIndexes[player->color - 1];

    bool can_buy =
        // if buyable by player materials and bank/harbor deals
        buyableProduct(ast, player, state->graph,
                       (signed char *)player->materials, player->harbors, store[SETTLEMENT], queue)
        // if player have amount
        && player->amounts[SETTLEMENT]
        // if player have buyable settlements
        && buyableSettlements(&stk, player, state->graph, true)
        // if bank have materials
        && (vector_manip_condition(state->bankMaterials, store[SETTLEMENT], TOTAL_MATERIALS, vector_sub, above_equal_zero) & 0x01);

    stack_destroy(&stk);

    return can_buy;
}
bool state_can_buy_city(PlayerPtr player, GameState state, QueuePtr queue)
{
    putts("state_can_buy_city");

    Stack stk;
    stack_init(&stk);

    unsigned char ast = state->astIndexes[player->color - 1];

    bool can_buy =
        // if buyable by player materials and bank/harbor deals
        buyableProduct(ast, player, state->graph, (signed char *)player->materials, player->harbors, store[CITY], queue)
        // if player have amount
        && player->amounts[CITY]
        // if have settlements to upgrade
        && upgradeableSettlements(&stk, player, state->graph)
        // if bank have materials
        && (vector_manip_condition(state->bankMaterials, store[CITY], TOTAL_MATERIALS, vector_sub, above_equal_zero) & 0x01);

    stack_destroy(&stk);

    return can_buy;
}
bool state_can_buy_development(PlayerPtr player, GameState state, QueuePtr queue)
{
    putts("state_can_buy_development");

    unsigned char ast = state->astIndexes[player->color - 1];

    print_vec(player->materials, TOTAL_MATERIALS);

    bool can_buy =
        // if buyable by player materials and bank/harbor deals
        buyableProduct(ast, player, state->graph, (signed char *)player->materials, player->harbors, store[DEVELOPMENT_CARD], queue)
        // if bank have developments
        && vector_sum(state->bankDevelopments, TOTAL_DEVELOPMENT_CARD)
        // if bank have materials
        && (vector_manip_condition(state->bankMaterials, store[DEVELOPMENT_CARD], TOTAL_MATERIALS, vector_sub, above_equal_zero) & 0x01);

    return can_buy;
}

// action functions

void handle_deal_num(PlayerPtr player, int socket,
                     GameState state, unsigned char deal_num)

{

    putts("handle_deal_num");

    unsigned char deal_mat_from = (deal_num & 0x07) - 1;
    unsigned char deal_type = (deal_num >> 3) & 0x03;
    unsigned char deal_mat_to = deal_num >> 5;

    socket_short_log(socket, 8 + (deal_type == 2));

    handle_deal(player, state, deal_mat_from, deal_mat_to, deal_type);
}
void handle_deal(PlayerPtr player, GameState state, unsigned char deal_mat_from, unsigned char deal_mat_to, unsigned deal_type)
{
    signed char cost_from_player[TOTAL_MATERIALS] = {0},
                cost_to_player[TOTAL_MATERIALS] = {0};

    cost_from_player[deal_mat_from] = deal_type + 2;
    cost_to_player[deal_mat_to]++;
    putts("transfer_materials handle deal");
    transfer_materials(player, state->bankMaterials, cost_from_player, false);
    transfer_materials(player, state->bankMaterials, cost_to_player, true);
}
void handle_action_q(PlayerPtr player,
                     int socket,
                     GameState state,
                     QueuePtr actionsQ)
{
    putts("handle_action_q");

    while (!queue_empty(*actionsQ))
    {
        handle_deal_num(player, socket, state,
                        convert_void_ptr_to_unsigned_char(dequeue(actionsQ)));
    }
}

void state_buy_road(PlayerPtr player,
                    int socket,
                    GameState state,
                    QueuePtr actionsQ)
{
    putts("state_buy_road");

    handle_action_q(player, socket, state, actionsQ);

    Stack stk;
    stack_init(&stk);

    buyableRoads(state->graph, &stk, player);
    unsigned char size, ast = state->astIndexes[player->color - 1];

    unsigned short (*prioritiseRoads[])(GraphPtr, PlayerPtr, Heap *, StackPtr) =
        {prioritiseWoodRoad, prioritiseWheatCardsRoad};

    unsigned short road = prioritiseRoads[!!ast](state->graph, player, state->astHeaps,
                                                 &stk);

    buy_road(player, state->graph, store[ROAD],
             state->bankMaterials, true, road & 0xFF, road >> 8);

    update_longest_road(state->graph, state->achievementCards + LONGEST_PATH);

    unsigned char *buffer = calloc(size = 3, sizeof(unsigned char));
    buffer[0] = 2;
    buffer[1] = road & 0xFF;
    buffer[2] = road >> 8;

    BOT_SEND_FREE(socket, size, buffer);
    socket_short_log(socket, 0);
}
void state_buy_settlement(PlayerPtr player,
                          int socket,
                          GameState state,
                          QueuePtr actionsQ)
{
    putts("state_buy_settlement");
    handle_action_q(player, socket, state, actionsQ);

    unsigned char size, ast = state->astIndexes[player->color - 1];

    unsigned char (*prioritiseSettlement[])(GraphPtr, PlayerPtr, Heap *) =
        {prioritiseWoodSettlement, prioritiseWheatCardsSettlement};

    unsigned char vertex = prioritiseSettlement[!!ast](state->graph, player, state->astHeaps);

    buy_settlement(player, state->graph, store[SETTLEMENT],
                   state->bankMaterials, -state->turnOffset / state->num_of_players, vertex);

    unsigned char *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 1;
    buffer[1] = vertex;

    BOT_SEND_FREE(socket, size, buffer);

    socket_short_log(socket, 1);
}
void state_buy_city(PlayerPtr player,
                    int socket,
                    GameState state,
                    QueuePtr actionsQ)
{
    putts("state_buy_city");

    handle_action_q(player, socket, state, actionsQ);

    unsigned char size, ast = state->astIndexes[player->color - 1];

    float (*scoreFn[])(float *) = {woodScore, wheatScore, cardsScore};

    unsigned char vertex = prioritiseUpgradeableSettlement(player, state->graph, scoreFn[ast]);

    buy_city(player, state->graph, store[CITY],
             state->bankMaterials, vertex);

    unsigned char *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 4;
    buffer[1] = vertex;

    BOT_SEND_FREE(socket, size, buffer);

    socket_short_log(socket, 2);
}
void state_buy_development(PlayerPtr player,
                           int socket,
                           GameState state,
                           QueuePtr actionsQ)
{
    putts("state_buy_development");

    handle_action_q(player, socket, state, actionsQ);

    socket_short_log(socket, 3);

    unsigned char index = buy_developement(player, state->bankMaterials, state->bankDevelopments, store[DEVELOPMENT_CARD]);

    void (*use_development[])(PlayerPtr, int, GameState) = {
        use_dev_knight,
        use_dev_point,
        use_dev_roads,
        use_dev_yop,
        use_dev_monopol};

    use_development[index](player, socket, state);

    signed char cost[TOTAL_DEVELOPMENT_CARD] = {0};
    cost[index] += index != 1;

    transfer_dev_card(player, state->bankDevelopments, cost, false);
}

bool state_steal(PlayerPtr player, int socket, GameState state)
{
    unsigned char size, area = moveRobberTo(player, state->graph, state->robberArea);
    state->robberArea = area;

    unsigned char players_around = players_around_area(&size, (signed char *)&area,
                                                       socket, state)[0];
    bool players[MAX_PLAYERS] = {false};
    size = MAX_PLAYERS;
    while (--size < MAX_PLAYERS)
        players[size] = (players_around >> size) & 0x01;

    unsigned char target_index = random_index_by_vals(MAX_PLAYERS, (signed char *)players);
    bool found = target_index < MAX_PLAYERS;

    unsigned char cost[TOTAL_MATERIALS] = {0};
    cost[random_index_by_vals(TOTAL_MATERIALS,
                              (signed char *)state->players[target_index].materials)] += found;

    putts("transfer_materials state steal");
    transfer_materials(player,
                       (signed char *)state->players[target_index].materials,
                       (signed char *)cost, true);
    //  update user
    unsigned char *buffer = calloc(size = 2, sizeof(unsigned char));
    buffer[0] = 6;
    buffer[1] = area;

    BOT_SEND_FREE(socket, size, buffer);

    socket_short_log(socket, 4);

    return true;
}