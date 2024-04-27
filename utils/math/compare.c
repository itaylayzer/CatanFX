#include "compare.h"
char bsign(char x)
{
    return (((x >> 7) & 0x01) * 2 - 1) * -1;
}
char babs(char x)
{
    return bsign(x) * x;
}
char bmin(char x, char y)
{
    return (x + y - babs(x - y)) / 2;
}
char bmax(char x, char y)
{
    return (x + y + babs(x - y)) / 2;
}