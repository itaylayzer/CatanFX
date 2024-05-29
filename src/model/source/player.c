#include "../headers/player.h"

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
VertexPtr moveRobberTo(PlayerPtr player, GraphPtr graph)
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
            heap_insert(&scores, vertex, score, heap_max);
        }
    }
    return heap_extract(&scores, heap_max);
}

Stack buyableRoads(PlayerPtr player)
{
    Stack stk;
    stack_init(&stk);
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
        stack_push(&stk, dest_edge);
    }
    QUEUE_TRAVARSE_FINISH;
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
Stack buyableSettlements(PlayerPtr player, GraphPtr graph, bool needRoad)
{
    Stack stk;
    stack_init(&stk);

    unsigned char offset;
    VertexPtr vertex;

    for (offset = 0; offset < VERTECIES; offset++)
    {
        vertex = graph->vertices + offset + AREAS;

        if (vertex->color == BLACK && buyableSettlement(player, vertex, needRoad))
        {
            stack_push(&stk, vertex);
        }
    }

    return stk;
}
Stack upgradeableSettlements(PlayerPtr player, GraphPtr graph)
{
    unsigned char v;

    Stack stk;
    stack_init(&stk);

    Node node;

    QUEUE_TRAVARSE(player->settlements, node);

    v = (unsigned char)node->data;

    // if settlement vertex and dont have house
    if (v >= AREAS && graph->vertices[v].color >> 6 == 0)
    {
        stack_push(&stk, graph->vertices + v);
    }
    QUEUE_TRAVARSE_FINISH;

    return stk;
}
bool buyable_product(signed char playerMats[TOTAL_MATERIALS],
                     signed char playerHarbors,
                     unsigned char productMats[TOTAL_MATERIALS],
                     StackPtr actionsStack)
{

    // תנאי עצירה
    if (vector_any(playerMats, TOTAL_MATERIALS, below_zero))
    {
        return false;
    }

    bool res = false;

    signed char *sub = vector_sub(playerMats, (signed char *)productMats, TOTAL_MATERIALS);
    if (vector_all(sub, TOTAL_MATERIALS, above_equal_zero))
    {
        free(sub);
        res = true;
    }

    unsigned char harbor, take, missingItem = vector_min_index(sub, TOTAL_MATERIALS);
    signed char *copy = malloc(sizeof(signed char) * TOTAL_MATERIALS);

    for (harbor = 0; harbor < TOTAL_MATERIALS && !res; harbor++)
    {
        vector_cpy(copy, playerMats, TOTAL_MATERIALS);
        if (playerHarbors >> harbor == 1 && playerMats[harbor] - 2 >= 0)
        {
            copy[harbor] -= 2;
            copy[missingItem] += 1;
            if (buyable_product(copy, playerHarbors, productMats, actionsStack))
            {
                stack_push(actionsStack, conver_unsigned_char_to_void_ptr(harbor));
                res = true;
            }
        }

        vector_cpy(copy, playerMats, TOTAL_MATERIALS);
        take = 4;
        if (playerHarbors >> 5 == 1)
        {
            take = 3;
        }
        if (playerMats[harbor] - take >= 0 && !res)
        {
            copy[harbor] -= take;
            copy[missingItem] += 1;
            if (buyable_product(copy, playerHarbors, productMats, actionsStack))
            {
                stack_push(actionsStack, conver_unsigned_char_to_void_ptr(harbor));
                res = true;
            }
        }
    }

    return res;
}