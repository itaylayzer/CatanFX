#if defined(_WIN32) || defined(_WIN64)
#ifndef _INC_WINDOWS_SERVER

#define _INC_WINDOWS_SERVER

int bserver_listen(void (*handle_request)(char *, int socket));

#endif
#endif