#include "controller.h"

void handle_request(
    signed char *input_buffer,
    int socket,
    GraphPtr graph,
    unsigned char *harbors,
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
    unsigned char size = 0;
    void *send_buffer;

    switch (input_buffer[0])
    {
    case 0:
        send_buffer = areas_numbers(&size, graph);
        break;
    case 1:
        send_buffer = harbors_numbers(&size, graph, harbors);
        break;

    case 2:
        send_buffer = players_around_area(&size, input_buffer[1], graph);
        break;

    case 10: // player actionable
        send_buffer = inf_player_actionable(&size, players, bankDevelopments, store);
        break;

    case 11: // player materials
        send_buffer = inf_player_materials(&size, players, bankMaterials, input_buffer[1]);
        break;

    case 12: // developements cards
        send_buffer = inf_player_devcards(&size, players, bankDevelopments,
                                          input_buffer[1]);

        break;

    case 13: // victory points
        send_buffer = single_byte(&size, (players + input_buffer[1])->victoryPoints);
        break;

    case 14: // player materials
        send_buffer = inf_player_amounts(&size, players);
        break;

    case 15: // achievement cards
        send_buffer = vector_dup(achievementCards, (size = TOTAL_ACHIEVEMENTS_CARD));
        break;

    case 30: // roll dice
        send_buffer = roll_dice(&size, players, graph, bankMaterials, *robberArea);
        break;

    case 31: // buy from store
        send_buffer = switch_action_store(&size,
                                          input_buffer + 1,
                                          graph,
                                          players,
                                          bankMaterials,
                                          bankDevelopments,
                                          store,
                                          achievementCards + LONGEST_PATH);
        break;
    case 32: // move robber
        send_buffer = move_robber(&size,
                                  players,
                                  0,
                                  robberArea,
                                  input_buffer + 1,
                                  achievementCards + BIGGEST_ARMY,
                                  num_of_players);
        break;
    case 33: // use dev card
        send_buffer = switch_dev_card(&size, players, num_of_players, bankMaterials, input_buffer + 1);
        break;

    case 34: // drop materials
        send_buffer = drop_materials(&size, players, (signed char *)(input_buffer + 1));
        break;

    case 40: // computer player play
        handle_rest_turns(socket,
                          graph,
                          players,
                          bankDevelopments,
                          bankMaterials,
                          turnOffset,
                          num_of_players,
                          achievementCards,
                          robberArea,
                          astHeaps,
                          astIndexes);
        size = 1;
        send_buffer = calloc(1, sizeof(char));
        break;

    default:
        return;
    }

    send(socket, &size, 1, 0);
    send(socket, send_buffer, size, 0);
    free(send_buffer);
}

void catan_start(signed char _num_of_players)
{

    // initialize game variables
    signed char num_of_players;
    PlayerRec *players;
    signed char bankMaterials[TOTAL_MATERIALS] = {19, 19, 19, 19, 19};
    signed char bankDevelopments[TOTAL_DEVELOPMENT_CARD] = {14, 5, 2, 2, 2};
    signed char achievementCards[TOTAL_ACHIEVEMENTS_CARD] = {-1, -1};
    unsigned char turnOffset = 0, robberArea = 9, *astIndexes;
    Heap astHeaps[TOTAL_ASTRATEGIES];
    GraphPtr graph;

    unsigned char harbors[HARBOR_COUNT * 2] = {0, 3, 1, 5, 10, 15, 26, 32, 42, 46, 49, 52, 47, 51, 33, 38, 11, 16};

    // change random seed to current time
    srand(time(NULL));

    // num of players
    players = malloc((num_of_players = _num_of_players) * sizeof(PlayerRec));

    // initialize graph
    graph_init(&graph);
    catan_graph_init(graph, harbors);

    catan_players_init(players, num_of_players);
    astIndexes = astrategies_init(graph, astHeaps);

    // initialize server
    server_listen(handle_request,
                  graph,
                  harbors,
                  players,
                  bankDevelopments,
                  bankMaterials,

                  &turnOffset,
                  num_of_players,
                  achievementCards,
                  &robberArea,
                  astHeaps,
                  astIndexes);
}