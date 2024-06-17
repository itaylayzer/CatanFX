#ifndef _INC_MODEL

#define _INC_MODEL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "../../utils/printt/printt.h"
#include "../../controller/types.h"
#include "../../controller/state.h"
#include "../../controller/constants.h"
#include "../../structures/graph/graph.h"
#include "../../structures/queue/queue.h"
#include "../../utils/math/compare.h"
#include "../../server/sock.h"
#include "../../utils/math/rand.h"
#include "../../utils/convert/convert.h"
#include "../../utils/vector/vector.h"
#include "./macro.h"
#include "./player.h"
#include "./actions.h"

//////////////////////////////////// initializations /////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 catan graph init
//                                 ----------------
//
// General      : The functions initialize the catan graph from edges.byte
//
// Parameters   :
//          graph      - GraphPtr (Out)
//          harbors    - Vector (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void catan_graph_init(GraphPtr graph, unsigned char harbors[HARBOR_COUNT * 2]);

//----------------------------------------------------------------------------------------
//                                 catan players init
//                                 ------------------
//
// General      : The functions initialize catan players with defualt values
//
// Parameters   :
//          players    - Players Vector (Out)
//          size       - Vector Size (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void catan_players_init(PlayerPtr players, signed char size);

//////////////////////////////////// information ////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 area numbers
//                                 ------------
//
// General      : The functions returns to the socket the area vertecies informations
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *areas_numbers(unsigned char *size, signed char *params,
                             int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 harbors numbers
//                                 ---------------
//
// General      : The functions returns to the socket the harbors informations
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *harbors_numbers(unsigned char *size, signed char *params,
                               int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 players around area
//                                 -------------------
//
// General      : The functions returns to the socket the player around area vertex in
//                  Bitmap
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *players_around_area(unsigned char *size, signed char *params,
                                   int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 informative player actionable
//                                 -----------------------------
//
// General      : The functions returns to the socket what action does the player able to
//                  perform in a BitMap
//                  Bitmap
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_player_actionable(unsigned char *size, signed char *params,
                                     int socket, GameState state);
//----------------------------------------------------------------------------------------
//                                 informative player materials
//                                 ----------------------------
//
// General      : The functions returns to the socket the player materials
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_player_materials(unsigned char *size, signed char *params,
                                    int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 informative players devcards
//                                 ----------------------------
//
// General      : The functions returns to the socket the player devcards
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_player_devcards(unsigned char *size, signed char *params,
                                   int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 informative players victory points
//                                 ----------------------------------
//
// General      : The functions calculate and returns to the socket the
//                      player victory points
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_player_victory_points(unsigned char *size, signed char *params,
                                         int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 informative players amounts
//                                 ---------------------------
//
// General      : The functions returns to the socket the player available
//                  products amounts
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_player_amounts(unsigned char *size, signed char *params,
                                  int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 informative achivement cards
//                                 ---------------------------
//
// General      : The functions returns to the socket the game achivement cards
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *inf_achivement_cards(unsigned char *size, signed char *params,
                                    int socket, GameState state);

////////////////////////////////////// actions ///////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 roll dice
//                                 ---------
//
// General      : The functions rolls the dice, sending materials to player, and return
//                  the dice results to the user
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *roll_dice(unsigned char *size, signed char *params,
                         int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 switch action store
//                                 -------------------
//
// General      : The functions find the relative store action and apply it by the player
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *switch_action_store(unsigned char *size, signed char *params,
                                   int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 switch devcards action
//                                 ----------------------
//
// General      : The functions find the relative dev card and apply it by the player
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *switch_dev_card(unsigned char *size, signed char *params,
                               int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 collect materials
//                                 -----------------
//
// General      : The functions collect the materials right after rolling the dice, and
//                  give it to tha players
//
// Parameters   :
//          rolled_num      - Vector Size (In)
//          players         - Player Vector (In/Out)
//          graph           - GraphPtr (In)
//          bank            - Bank Materials (In/Out)
//          robberArea      - Robber Area Number (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void collect_materials(unsigned char rolled_num,
                       PlayerPtr players,
                       GraphPtr graph,
                       signed char *bank,
                       const unsigned char robberArea);
//----------------------------------------------------------------------------------------
//                                 move robber
//                                 -----------
//
// General      : The functions move the robber by the params
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *move_robber(unsigned char *size, signed char *params,
                           int socket, GameState state);
//----------------------------------------------------------------------------------------
//                                 drop materials
//                                 --------------
//
// General      : The functions drop materials for a player
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *drop_materials(unsigned char *size, signed char *params,
                              int socket, GameState state);
//----------------------------------------------------------------------------------------
//                                 make a deal
//                                 -----------
//
// General      : The functions makes a deal by the socket player
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *make_a_deal(unsigned char *size, signed char *params, int socket,
                           GameState state);

////////////////////////////////////// bots //////////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 bot drop materials
//                                 ------------------
//
// General      : The functions drop materials by bot player
//
// Parameters   :
//          PlayerPtr       - Bot Player (In/Out)
//          GameState       - Game State (In/Out)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool bot_drop_materials(PlayerPtr, GameState);

//----------------------------------------------------------------------------------------
//                                 handle rest turns
//                                 -----------------
//
// General      : The functions drop materials by bot player
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *handle_rest_turns(unsigned char *size, signed char *params,
                                 int socket, GameState state);
/////////////////////////////////////// closing //////////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 close server
//                                 ------------
//
// General      : The functions close the server
//
// Parameters   :
//          size       - Vector Size Pointer (Out)
//          params     - Parameters Vector (In)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : Response Vector
//
//----------------------------------------------------------------------------------------
unsigned char *close_server(unsigned char *size, signed char *params, int socket, GameState state);
#endif