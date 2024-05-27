#include "controller.h"

unsigned char *use_dev_card(unsigned char *size,
                            PlayerPtr player,
                            unsigned char offset,
                            unsigned char amount)
{
    player->developmentCards[offset] -= amount;
    return (unsigned char *)vec_dup(
        (signed char *)player->developmentCards,
        (*size = TOTAL_DEVELOPMENT_CARD));
}

void handle_request(
    signed char *buffer,
    int socket,
    GraphPtr graph,
    unsigned char *harbors,
    PlayerPtr players,
    signed char *bankDevelopments,
    signed char *bankMaterials,
    const signed char (*store)[TOTAL_MATERIALS],
    unsigned char *turnOffset,
    const unsigned char num_of_players,
    signed char *achievementCards,
    unsigned char *robberArea)
{
    unsigned char size = 0;
    void *_buff;

    switch (buffer[0])
    {
    case 0:
        _buff = areas_numbers(&size, graph);
        break;
    case 1:
        _buff = harbors_numbers(&size, graph, harbors);
        break;
    case 10:
        _buff = inf_player_actionable(&size, players, bankDevelopments, store);
        break;

    case 11: // player materials
        _buff = inf_player_materials(&size, players, bankMaterials, buffer[1]);
        break;

    case 12: // developements cards
        _buff = inf_player_devcards(&size, players, bankDevelopments, buffer[1]);
        break;

    case 13: // victory points
        _buff = single_byte(&size, (players + buffer[1])->victoryPoints);
        break;

    case 14: // player materials
        _buff = inf_player_amounts(&size, players);
        break;

    case 15: // achievement cards
        _buff = vec_dup(achievementCards, (size = TOTAL_ACHIEVEMENTS_CARD));
        break;

    case 30: // roll dice
        _buff = roll_dice(&size, players, graph, bankMaterials, *robberArea);
        break;

    case 31: // buy from store
        _buff = switch_action_store(&size,
                                    buffer + 1,
                                    graph,
                                    players,
                                    bankMaterials,
                                    bankDevelopments,
                                    store,
                                    achievementCards + LONGEST_PATH);
        break;
    case 32: // move robber
        _buff = move_robber(&size,
                            players,
                            0,
                            robberArea,
                            buffer + 1,
                            achievementCards + BIGGEST_ARMY,
                            num_of_players);
        break;
    case 33: // use dev card
        _buff = switch_dev_card(&size, players, num_of_players, bankMaterials, buffer + 1);
        break;

    case 40:
        handle_rest_turns(socket, turnOffset, players, num_of_players);
        size = 1;
        _buff = calloc(1, sizeof(char));
        break;

    default:
        return;
    }

    send(socket, &size, 1, 0);
    send(socket, _buff, size, 0);
    free(_buff);
}

void catan_start(signed char _num_of_players)
{

    // initialize game variables
    signed char num_of_players;
    PlayerRec *players;
    signed char bankMaterials[TOTAL_MATERIALS] = {19, 19, 19, 19, 19};
    signed char bankDevelopments[TOTAL_DEVELOPMENT_CARD] = {14, 5, 2, 2, 2};
    signed char achievementCards[TOTAL_ACHIEVEMENTS_CARD] = {-1, -1};
    unsigned char turnOffset = 0, robberArea = 9;
    GraphPtr graph;

    unsigned char harbors[HARBOR_COUNT * 2] = {0, 3, 1, 5, 10, 15, 26, 32, 42, 46, 49, 52, 47, 51, 33, 38, 11, 16};

    // change random seed to current time
    srand(time(NULL));

    puts("\n");
    // num of players
    players = malloc((num_of_players = _num_of_players) * sizeof(PlayerRec));

    // initialize graph
    graph_init(&graph);
    catan_graph_init(graph, harbors);

    catab_players_init(players, num_of_players);

    printt("done initializing catan: %d players\n", num_of_players);
    // initialize server
    server_listen(handle_request,
                  graph,
                  harbors,
                  players,
                  bankDevelopments,
                  bankMaterials,
                  store,
                  &turnOffset,
                  num_of_players,
                  achievementCards,
                  &robberArea);
}