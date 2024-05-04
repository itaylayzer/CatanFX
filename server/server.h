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

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

#elif defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#else
#error "Unsupported platform"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "../utils/printt/printt.h"

#define PORT 5900
#define BUFFER_SIZE 256

int server_listen(void (*handle_request)(
                      signed char *buffer,
                      int socket,
                      GraphPtr graph,
                      unsigned char *harbors, PlayerPtr players,
                      signed char *bankDevelopments,
                      signed char *bankMaterials,
                      const signed char (*store)[TOTAL_MATERIALS],
                      unsigned char *turnOffset,
                      const unsigned char num_of_players),
                  GraphPtr graph,
                  unsigned char *harbors, PlayerPtr players,
                  signed char *bankDevelopments,
                  signed char *bankMaterials,
                  const signed char (*store)[TOTAL_MATERIALS],
                  unsigned char *turnOffset,
                  const unsigned char num_of_players);
#endif