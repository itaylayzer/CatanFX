#include "../headers/player.h"

// helpers
bool below_zero(signed char x)
{
    return x < 0;
}
bool above_equal_zero(signed char x)
{
    return x >= 0;
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
    if (edge->color == GRAY)
    {
        stack_push(&stk, dest);
    }
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
    if ((dest->color & 0x0F) != BLACK)
    {
        dontHaveHouseNear = false;
    }
    if (needRoad && edge->color == player->color)
    {
        connectedByRoad = true;
    }
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

    for (settlement_offset = AREAS;
         settlement_offset < VERTECIES + AREAS;
         settlement_offset++)
    {
        float mats_probs[TOTAL_MATERIALS] = {0};

        Node node;
        EdgePtr edge;
        unsigned char mat_number;

        QUEUE_TRAVARSE(graph->vertices[settlement_offset].edges, node);

        edge = node->data;
        mat_number = edge->vertex->material_number;

        if (edge->color == GRAY)
        {
            mats_probs[mat_number & 0x07] += probs[(mat_number >> 3) - 2];
        }

        QUEUE_TRAVARSE_FINISH;

        heap_insert(astWood, convert_unsigned_char_to_void_ptr(settlement_offset),
                    woodScore(mats_probs), heap_max);
        heap_insert(astWheat, convert_unsigned_char_to_void_ptr(settlement_offset),
                    wheatScore(mats_probs), heap_max);
        heap_insert(astCards, convert_unsigned_char_to_void_ptr(settlement_offset),
                    cardsStore(mats_probs), heap_max);
    }
}
unsigned char getMissingMaterial(signed char playerMaterials[TOTAL_MATERIALS],
                                 signed char productMats[TOTAL_MATERIALS])
{
    const signed char *sub = vector_sub(playerMaterials, productMats, TOTAL_MATERIALS);
    const unsigned char min_index = vector_min_index(sub, TOTAL_MATERIALS);
    free(sub);
    return min_index;
}

unsigned char *leastImportent(unsigned char astrategy, PlayerPtr player, GraphPtr graph)
{
    unsigned char *(*func[3])(PlayerPtr, GraphPtr) = {woodMatsOrder,
                                                      wheatMatsOrder,
                                                      cardsMatsOrder};

    unsigned char *res = func[astrategy](player, graph);
    signed char *mats = vector_dup((signed char *)res + TOTAL_MATERIALS, TOTAL_MATERIALS);
    free(res);

    vector_reverse(mats, TOTAL_MATERIALS);

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

    for (harborOffset = 0; harborOffset < TOTAL_MATERIALS && !result; harborOffset++)
    {
        harbor = least_importent[harborOffset];
        if (playerHarbors >> harborOffset & 0x01 &&
            vector_all(playerMaterials, TOTAL_MATERIALS, above_equal_zero) && !result)
        {
            playerMaterials[harborOffset] -= 2;
            playerMaterials[missingMaterial]++;
            result = harborOffset + 1;
        }

        take = 4;
        if (playerHarbors >> 5)
        {
            take = 3;
        }

        if (playerMaterials[harbor] - take >= 0 && !result)
        {
            playerMaterials[harborOffset] -= take;
            playerMaterials[missingMaterial]++;
            result = harborOffset + 1 + (take - 2) >> 3;
        }
    }

    return result;
}
unsigned char find_first_true_index(bool *arr, unsigned char size)
{
    unsigned char offset = 0;
    while (!arr[offset])
        offset++;
    return offset;
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
float cardsStore(float mats[TOTAL_MATERIALS])
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

    while ((min_index = vector_min_index(scores, TOTAL_ASTRATEGIES)) &&
           scores[min_index] != S_MIN_VALUE &&
           --offset >= 0)
    {
        indexes[offset] = min_index;
        scores[min_index] = S_MIN_VALUE;
    }

    return indexes;
}

// astrategies prioritise functions
unsigned char *woodMatsOrder(PlayerPtr player, GraphPtr graph)
{
    Stack temp;
    stack_init(&temp);

    bool states[TOTAL_STORE] =
        {player->amounts[ROAD] && buyableRoads(&temp, player),
         player->amounts[SETTLEMENT] && buyableSettlements(&temp, player, graph, true),
         player->amounts[CITY] && upgradeableSettlements(&temp, player, graph),
         true};

    unsigned char values[TOTAL_STORE][TOTAL_MATERIALS * 2] =
        {{store[ROAD][0], store[ROAD][1], store[ROAD][2], store[ROAD][3], store[ROAD][4], BRICK, WOOD, ORE, WHEAT, WOOL},

         {store[SETTLEMENT][0], store[SETTLEMENT][1], store[SETTLEMENT][2], store[SETTLEMENT][3], store[SETTLEMENT][4], BRICK, WOOD, WHEAT, WOOL, ORE},

         {store[CITY][0], store[CITY][1], store[CITY][2], store[CITY][3], store[CITY][4], ORE, WHEAT, BRICK, WOOD, WOOL},

         {store[DEVELOPMENT_CARD][0], store[DEVELOPMENT_CARD][1], store[DEVELOPMENT_CARD][2], store[DEVELOPMENT_CARD][3], store[DEVELOPMENT_CARD][4], ORE, WHEAT, WOOL, BRICK, WOOD}};

    unsigned char index = find_first_true_index(states, TOTAL_STORE);
    unsigned char *res = vector_dup(values[index], TOTAL_MATERIALS * 2);
    stack_destroy(&temp);

    return res;
}
unsigned char *wheatMatsOrder(PlayerPtr player, GraphPtr graph)
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
    unsigned char *res = vector_dup(values[index], TOTAL_MATERIALS * 2);
    stack_destroy(&temp);

    return res;
}
unsigned char *cardsMatsOrder(PlayerPtr player, GraphPtr graph)
{
    unsigned char value[TOTAL_MATERIALS * 2] = {store[DEVELOPMENT_CARD][0],
                                                store[DEVELOPMENT_CARD][1],
                                                store[DEVELOPMENT_CARD][2],
                                                store[DEVELOPMENT_CARD][3],
                                                store[DEVELOPMENT_CARD][4],
                                                ORE < WHEAT, WOOL, BRICK, WOOD};
    return vector_dup(value, TOTAL_MATERIALS * 2);
}

// astrategies play function
// TODO:

// generic functions
unsigned char prioritiseUpgradeableSettlement(PlayerPtr player, GraphPtr graph,
                                              float (*scoreFn)(float[TOTAL_MATERIALS]))
{
    Node node;
    EdgePtr edge;
    VertexPtr vertex;
    Heap scores;
    heap_init(&scores, VERTECIES);

    unsigned char mat_number, vertex_index;

    QUEUE_TRAVARSE(player->settlements, node);

    vertex_index = convert_void_ptr_to_unsigned_char(node->data);
    vertex = graph->vertices + vertex_index;

    float mats_probs[TOTAL_MATERIALS] = {0};

    QUEUE_TRAVARSE(vertex->edges, node);

    edge = node->data;
    mat_number = edge->vertex->material_number;
    if (edge->color == GRAY)
    {
        mats_probs[mat_number & 0x07] += probs[(mat_number >> 3) - 2];
    }

    QUEUE_TRAVARSE_FINISH;

    heap_insert(&scores, convert_unsigned_char_to_void_ptr(vertex_index),
                scoreFn(mats_probs), heap_max);

    QUEUE_TRAVARSE_FINISH;

    const void *vertexOffset = heap_extract(&scores, heap_max, NULL);
    return convert_void_ptr_to_unsigned_char(vertexOffset);
}
unsigned char moveRobberTo(PlayerPtr player, GraphPtr graph)
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

            if (settlement->color < BLACK)
            {
                score += prob;
            }
        }

        if (belongToPlayer == false)
        {
            heap_insert(&scores, convert_unsigned_char_to_void_ptr(offset), score, heap_max);
        }
    }

    const void *vertexOffset = heap_extract(&scores, heap_max, NULL);
    return convert_void_ptr_to_unsigned_char(vertexOffset);
}
unsigned char buyableRoads(StackPtr stk, PlayerPtr player)
{

    unsigned char count = 0;
    Queue sourceQ, destQ;
    Node node;
    VertexPtr dest;
    EdgePtr dest_edge;
    queue_init(&sourceQ);
    queue_init(&destQ);

    enqueue(&sourceQ, player->roads);

    QUEUE_TRAVARSE_BODY(sourceQ, node);
    dest = ((EdgePtr)node->data)->vertex;
    enqueue(&destQ, dest->edges);

    QUEUE_TRAVARSE_BODY(destQ, node);

    dest_edge = node->data;

    if (dest_edge->color == BLACK)
    {
        stack_push(stk, dest_edge);
        count++;
    }
    QUEUE_TRAVARSE_FINISH;
    QUEUE_TRAVARSE_FINISH;
    return count;
}
unsigned char buyableSettlements(StackPtr stk, PlayerPtr player, GraphPtr graph, bool needRoad)
{
    unsigned char count = 0;

    unsigned char offset;
    VertexPtr vertex;

    for (offset = 0; offset < VERTECIES; offset++)
    {
        vertex = graph->vertices + offset + AREAS;

        if (vertex->color == BLACK && buyableSettlement(player, vertex, needRoad))
        {
            count++;
            stack_push(stk, vertex);
        }
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
    if (vertex >= AREAS && graph->vertices[vertex].color >> 6 == 0)
    {
        stack_push(stk, graph->vertices + vertex);
    }
    QUEUE_TRAVARSE_FINISH;

    return count;
}
bool buyableProduct(unsigned char astrategy,
                    PlayerPtr player,
                    GraphPtr graph,
                    signed char playerMats[TOTAL_MATERIALS],
                    signed char playerHarbors,
                    unsigned char productMats[TOTAL_MATERIALS],
                    QueuePtr actionsQ)
{

    signed char *clone, *sub;
    unsigned char res, missingMat;
    bool buyable;

    clone = vector_dup(playerMats, TOTAL_MATERIALS);
    missingMat = getMissingMaterial(clone, productMats);
    res = buyableMaterial(astrategy, player, graph, missingMat, clone, playerHarbors);
    sub = vector_sub(clone, productMats, TOTAL_MATERIALS);

    while (!(buyable = vector_all(sub, TOTAL_MATERIALS, above_equal_zero)) && !res)
    {

        enqueue(actionsQ, convert_unsigned_char_to_void_ptr(res));
        missingMat = getMissingMaterial(clone, productMats);
        res = buyableMaterial(astrategy, player, graph, missingMat, clone, playerHarbors);

        free(sub);
        sub = vector_sub(clone, productMats, TOTAL_MATERIALS);
    }

    return buyable;
}