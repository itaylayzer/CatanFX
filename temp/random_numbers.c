#include <time.h>
#include <stdlib.h>
#include <stdio.h>

char bsign(char x)
{
    return (((x >> 7) & 0x01) * 2 - 1) * -1;
}
char babs(char x)
{
    return bsign(x) * x;
}

int main()
{

    unsigned char arr[1];
    srand(time(NULL));
    char value = (babs(rand()) % 12) + 1;

    arr[0] = 0;
    arr[0] = value << 4;

    value = (babs(rand()) % 12) + 1;
    arr[0] |= value;

    printf("%d %d\n", arr[0] & 0x0F, arr[0] >> 4);

    return 1;
}