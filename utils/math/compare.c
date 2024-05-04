#include "compare.h"
signed char bsign(signed char x)
{
    return (((x >> 7) & 0x01) * 2 - 1) * -1;
}
signed char babs(signed char x)
{
    return bsign(x) * x;
}
signed char bmin(signed char x, signed char y)
{
    return (x + y - babs(x - y)) / 2;
}
signed char bmax(signed char x, signed char y)
{
    return (x + y + babs(x - y)) / 2;
}