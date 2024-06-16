#ifndef _INC_CONTROLLER

#define _INC_CONTROLLER

#include "./types.h"
#include "../model/headers/model.h"
#include "../model/headers/player.h"
#include "../structures/graph/graph.h"
#include "../server/server.h"
#include "../utils/printt/printt.h"
#include "./constants.h"
#include "./state.h"

//----------------------------------------------------------------------------------------
//                                 catan start
//                                 -----------
//
// General      : The functions start the game server.
//
// Parameters   :
//          signed char - how many players (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void catan_start(signed char);

#endif