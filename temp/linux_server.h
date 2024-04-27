#if defined(__linux__)
#ifndef _INC_LINUX_SERVER

#define _INC_LINUX_SERVER

#include <sys/socket.h>
#include <netinet/in.h>

#include "shared.h"

int aserver_listen(void (*handle_request)(char *, int socket));
#endif
#endif