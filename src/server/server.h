// #ifndef _INC_SERVER

// #define _INC_SERVER

// #if defined(_WIN32) || defined(_WIN64)
// #include "windows_server.h"
// #elif defined(__linux__)
// #include "linux_server.h"
// #else
// #error "Unsupported platform"
// #endif

// #endif

#ifndef _INC_SERVER

#define _INC_SERVER

#include <stdio.h>
#include <stdlib.h>

#include "./sock.h"
#include "../controller/types.h"
#include "../utils/printt/printt.h"
#include "../structures/heap/heap.h"
#define PORT 5173
#define BUFFER_SIZE 256

int server_listen(void (*handle_request)(
                      signed char *buffer,
                      int socket,
                      GraphPtr graph,
                      unsigned char *harbors, PlayerPtr players,
                      signed char *bankDevelopments,
                      signed char *bankMaterials,
                      unsigned char *turnOffset,
                      const unsigned char num_of_players,
                      signed char *achievementCards,
                      unsigned char *robberArea,
                      Heap heaps[TOTAL_ASTRATEGIES],
                      unsigned char *astIndexes),
                  GraphPtr graph,
                  unsigned char *harbors, PlayerPtr players,
                  signed char *bankDevelopments,
                  signed char *bankMaterials,
                  unsigned char *turnOffset,
                  const unsigned char num_of_players,
                  signed char *achievementCards,
                  unsigned char *robberArea,
                  Heap heaps[TOTAL_ASTRATEGIES],
                  unsigned char *astIndexes);
#endif