#ifndef _INC_SERVER_SOCK

#define _INC_SERVER_SOCK

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

#elif defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#else

#error "Unsupported platform"

#endif

#endif