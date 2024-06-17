#ifndef _INC_ACTIONS

#define _INC_ACTIONS

#include "../../controller/controller.h"

//----------------------------------------------------------------------------------------
//                                 Value Compare
//                                 -------------
//
// General      : Compare Functions that compare 2 8 Bytes Values
//
// Parameters   :
//          first      - 8 Byte Value (In)
//          second     - 8 Byte Value (In)
//
// Return Value : Compare Value
//
//----------------------------------------------------------------------------------------
signed char value_compare(const void *first, const void *second);

//----------------------------------------------------------------------------------------
//                                 transfer materials
//                                 ------------------
//
// General      : The functions transfer materials between player and a bank
//
// Parameters   :
//          player     - Player (In/Out)
//          bank       - Bank Materials (In/Out)
//          cost       - Vector (In)
//          to_player  - Transfer To Player boolean (In)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool transfer_materials(PlayerPtr player,
                        signed char bank[TOTAL_MATERIALS],
                        const signed char cost[TOTAL_MATERIALS],
                        bool to_player);

//----------------------------------------------------------------------------------------
//                                 transfer all players mats
//                                 -------------------------
//
// General      : The functions transfer all players specific material to another player
//
// Parameters   :
//          players         - Player Vector (In/Out)
//          player_index    - To Player Index (In)
//          num_of_player   - Vector Size (In)
//          mat             - Material Number (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void transfer_all_players_mats(PlayerPtr players,
                               unsigned char player_index,
                               signed char num_of_players,
                               unsigned char mat);

//----------------------------------------------------------------------------------------
//                                 update longest road
//                                 -------------------
//
// General      : The functions update the longest road achivement card
//
// Parameters   :
//          graph                   - GraphPtr (In)
//          longest_road_achivement - Number Pointer (In/Out)
//
// Return Value : Longest Road Achivement Number
//
//----------------------------------------------------------------------------------------
signed char update_longest_road(GraphPtr graph, signed char *longest_road_achievement);

//----------------------------------------------------------------------------------------
//                                 update biggest army
//                                 -------------------
//
// General      : The functions update the biggest army achivement card
//
// Parameters   :
//          players                     - Players Vector (In)
//          num_of_players              - Vector Size (In)
//          biggest_army_achivement     - Number Pointer (In/Out)
//
// Return Value : Biggest Army Achivement Number
//
//----------------------------------------------------------------------------------------
signed char update_biggest_army(PlayerPtr players,
                                signed char num_of_players,
                                signed char *biggest_army_achievement);

//----------------------------------------------------------------------------------------
//                                 buy road
//                                 --------
//
// General      : The functions buys a road for a player
//
// Parameters   :
//          player          - Player (In/Out)
//          graph           - GraphPtr (In/Out)
//          cost            - Vector (In)
//          bank            - Vector (In/Out)
//          transferMats    - Do Transfer to bank boolean (In)
//          from            - from vertex id (In)
//          to              - to vertex id (In)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool buy_road(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              bool transferMats,
              unsigned char from,
              unsigned char to);

//----------------------------------------------------------------------------------------
//                                 change road
//                                 -----------
//
// General      : The functions changes a road color
//
// Parameters   :
//          color      - Color index (In)
//          graph      - GraphPtr (In/Out)
//          from       - from vertex id (In)
//          to         - to vertex id (In)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool change_road(unsigned char color,
                 GraphPtr graph,
                 unsigned char from,
                 unsigned char to);

//----------------------------------------------------------------------------------------
//                                 buy city
//                                 --------
//
// General      : The functions buys a city for a player
//
// Parameters   :
//          player     - Player (In/Out)
//          graph      - GraphPtr (In/Out)
//          cost       - Vector (In)
//          bank       - Bank Materials Vector (In/Out)
//          index      - vertex id (In)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool buy_city(PlayerPtr player,
              GraphPtr graph,
              const signed char cost[TOTAL_MATERIALS],
              signed char bank[TOTAL_MATERIALS],
              unsigned char index);
//----------------------------------------------------------------------------------------
//                                 transfer dev card
//                                 -----------------
//
// General      : The functions transfer devcards between player and a bank
//
// Parameters   :
//          player     - Player (In/Out)
//          bank       - Bank Development cards (In/Out)
//          cost       - Vector (In)
//          to_player  - Transfer To Player boolean (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void transfer_dev_card(PlayerPtr player,
                       signed char bank[TOTAL_DEVELOPMENT_CARD],
                       const signed char *cost,
                       bool to_player);

//----------------------------------------------------------------------------------------
//                                 buy development card
//                                 --------------------
//
// General      : The functions buy a development card to a player
//
// Parameters   :
//          player     - Player (In/Out)
//          bank       - Bank Materials Vector (In/Out)
//          dev_bank   - Bank Developments Vector (In/Out)
//          cost       - Vector (In)
//
// Return Value : Development Card type number
//
//----------------------------------------------------------------------------------------
unsigned char buy_developement(PlayerPtr player,
                               signed char bank[TOTAL_MATERIALS],
                               signed char dev_bank[TOTAL_DEVELOPMENT_CARD],
                               const signed char cost[TOTAL_MATERIALS]);

//----------------------------------------------------------------------------------------
//                                 random index by values
//                                 ----------------------
//
// General      : The functions calculate a random index based of the values and
//                  not the size
//
// Parameters   :
//          size       - Vector Size (In)
//          arr        - Vecot (In)
//
// Return Value : Random index number
//
//----------------------------------------------------------------------------------------
unsigned char random_index_by_vals(unsigned char size, signed char *arr);

//----------------------------------------------------------------------------------------
//                                 buy city
//                                 --------
//
// General      : The functions buys a settlement for a player
//
// Parameters   :
//          player          - Player (In/Out)
//          graph           - GraphPtr (In/Out)
//          cost            - Vector (In)
//          bank            - Bank Materials Vector (In/Out)
//          transfermats    - 1 take mats from player, -1 give mats to player (In)
//          index           - vertex id (In)
//
// Return Value : True
//
//----------------------------------------------------------------------------------------
bool buy_settlement(PlayerPtr player,
                    GraphPtr graph,
                    const signed char cost[TOTAL_MATERIALS],
                    signed char bank[TOTAL_MATERIALS],
                    signed char transferMats, // -1 to player 1 from player
                    unsigned char index);

//----------------------------------------------------------------------------------------
//                                 extract area materials
//                                 ----------------------
//
// General      : The functions extract materials related to an area vertex
//
// Parameters   :
//          material_number - Vertex Variable (In)
//
// Return Value : Material number
//
//----------------------------------------------------------------------------------------
unsigned char extract_area_materials(unsigned char material_number);

/////////////////////////////////// use dev_card bot actions /////////////////////////////

//----------------------------------------------------------------------------------------
//                                 use dev knight
//                                 --------------
//
// General      : The functions uses a dev knight for a bot player and notify the socket
//
// Parameters   :
//          player     - Bot Player (In/Out)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void use_dev_knight(PlayerPtr player, int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 use dev victory point
//                                 ---------------------
//
// General      : The functions uses a dev victory point for a bot player and notify
//                 the socket
//
// Parameters   :
//          player     - Bot Player (In/Out)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void use_dev_point(PlayerPtr player, int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 use dev 2 roads
//                                 ---------------
//
// General      : The functions uses a dev roads for a bot player and notify the socket
//
// Parameters   :
//          player     - Bot Player (In/Out)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void use_dev_roads(PlayerPtr player, int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 use dev year of plants
//                                 ----------------------
//
// General      : The functions uses a dev year of plants for a bot player and
//                  notify the socket
//
// Parameters   :
//          player     - Bot Player (In/Out)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void use_dev_yop(PlayerPtr player, int socket, GameState state);

//----------------------------------------------------------------------------------------
//                                 use dev monopol
//                                 ---------------
//
// General      : The functions uses a dev monopol for a bot player and notify the socket
//
// Parameters   :
//          player     - Bot Player (In/Out)
//          socket     - Socket (In)
//          state      - Game State (In/Out)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void use_dev_monopol(PlayerPtr player, int socket, GameState state);

/////////////////////////////////// socket shortcuts /////////////////////////////////////

//----------------------------------------------------------------------------------------
//                                 socket shortcut update
//                                 ----------------------
//
// General      : The functions sends to a socket update signal
//
// Parameters   :
//          socket     - Socket (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void socket_short_update(int socket);

//----------------------------------------------------------------------------------------
//                                 socket shortcut log
//                                 -------------------
//
// General      : The functions sens to a socket log signal
//
// Parameters   :
//          socket     - Socket (In)
//          log        - Number (In)
//
// Return Value : None
//
//----------------------------------------------------------------------------------------
void socket_short_log(int socket, unsigned char log);

#endif