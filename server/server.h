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

#define PORT 1050
#define BUFFER_SIZE 256

int server_listen(void (*handle_request)(char *, int socket));

#endif