#ifndef _INC_SERVER

#define _INC_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./sock.h"
#include "../controller/types.h"
#include "../utils/printt/printt.h"
#include "../structures/heap/heap.h"
#include "../controller/state.h"
#include "../utils/vector/vector.h"

#define PORT 5173
#define BUFFER_SIZE 256

//----------------------------------------------------------------------------------------
//                                 server listen
//                                 -------------
//
// General      : The functions starts a server and attach a request handler.
//
// Parameters   :
//          handle_request - request handler (In)
//          state      - Game State (In/Out)
//
// Return Value : Exit status integer.
//
//----------------------------------------------------------------------------------------
int server_listen(void (*handle_request)(
                      signed char *buffer,
                      int socket,
                      GameState state),
                  GameState state);

#endif