#include "./rand.h"
unsigned char brand(unsigned char min, unsigned char max)
{
    return (babs(rand()) % (max - min)) + min;
}