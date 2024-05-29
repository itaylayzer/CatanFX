#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main()
{
    puts("before");
    usleep(300000);
    puts("after");
}
