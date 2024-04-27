#include "controller.h"

// initialize game variables
char num_of_players = 3;

PlayerRec *players;

char bankMaterials[TOTAL_MATERIALS] = {19, 19, 19, 19, 19};

char bankDevelopments[TOTAL_DEVELOPMENT_CARD] = {14, 5, 2, 2, 2};

char achievementCards[TOTAL_ACHIEVEMENTS_CARD] = {-1, -1};

unsigned char robberArea = 9;

GraphPtr graph;

const char store[TOTAL_STORE][TOTAL_MATERIALS] = {
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 2, 0, 3},
    {0, 1, 1, 0, 1}};

unsigned char harbors[HARBOR_COUNT * 2] = {0, 3, 1, 5, 10, 15, 26, 32, 42, 46, 49, 52, 47, 51, 33, 38, 11, 16};

void handle_request(char *buffer, int socket)
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
        _buff = inf_player_materials(&size, players, buffer[1]);
        break;

    case 12: // developements cards
        _buff = inf_player_devcards(&size, players, buffer[1]);
        break;

    case 13: // victory points
        _buff = single_byte(&size, (players + buffer[1])->victoryPoints);
        break;
        
    case 30:
        _buff = roll_dice(&size, players, graph, bankMaterials);
        break;
    case 31:
        _buff = switch_action_store(&size,
                                    _buff + 1,
                                    graph,
                                    players,
                                    bankMaterials,
                                    bankDevelopments,
                                    store);
        break;
    default:
        return;
    }
    printf("action %d size=%d\n", buffer[0], size);
    send(socket, &size, 1, 0);
    send(socket, _buff, size, 0);
    free(_buff);
}

void print_time()
{
    time_t timer;
    char buffer[26];
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);
}

void catan_start(char _num_of_players)
{
    // change random seed to current time
    srand(time(NULL));

    puts(
        "");
    puts("");
    print_time();

    // num of players
    players = malloc((num_of_players = _num_of_players) * sizeof(PlayerRec));

    // initialize graph
    graph_init(&graph);
    catan_graph_init(graph, harbors);
    printf("num_of_players:%d\n", num_of_players);
    catab_players_init(players, num_of_players);
    printf("done:%d\n", num_of_players);

    // initialize server
    server_listen(handle_request);
}