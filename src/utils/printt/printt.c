#include "printt.h"

//----------------------------------------------------------------------------------------
//                                 print time
//                                 ----------
//
// General      : The function prints the time.
//
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void print_time()
{
    time_t timer;
    char buffer[26];
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "[%H:%M:%S] ", tm_info);
    printf("%s", buffer);
}

void printt(const char *format, ...)
{
    print_time();

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
void putts(const char *message)
{
    print_time();
    puts(message);
}