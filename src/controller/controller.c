#include "controller.h"

void handle_request(
    signed char *input_buffer,
    int socket,
    GameState state)
{
    unsigned char size = 0;
    void *send_buffer;

    unsigned char *(*func[])(unsigned char *, signed char *, int, GameState) =
        {areas_numbers,
         harbors_numbers,
         players_around_area,
         inf_player_actionable,
         inf_player_materials,
         inf_player_devcards,
         inf_player_victory_points,
         inf_player_amounts,
         inf_achivement_cards,
         roll_dice,
         switch_action_store,
         move_robber,
         switch_dev_card,
         drop_materials,
         handle_rest_turns};

    puts("|");
    printt("input_buffer[0]:%d\n", input_buffer[0]);

    send_buffer = func[input_buffer[0]](&size, input_buffer + 1, socket, state);

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
    const unsigned char turnOffset = 0, robberArea = 9;
    unsigned char *astIndexes;
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

    stateRec state = {graph,
                      harbors,
                      players,
                      bankDevelopments,
                      bankMaterials,
                      turnOffset,
                      num_of_players,
                      achievementCards,
                      robberArea,
                      astHeaps,
                      astIndexes};
    // initialize server
    server_listen(handle_request, &state);
}