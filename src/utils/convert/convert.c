
#include "./convert.h"

void *conver_unsigned_char_to_void_ptr(unsigned char value)
{
    void *val = NULL;
    *(unsigned char *)&val = value;
    return val;
}