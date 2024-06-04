
#include "./convert.h"

void *convert_unsigned_char_to_void_ptr(const unsigned char value)
{
    void *val = NULL;
    *(unsigned char *)&val = value;
    return val;
}

unsigned char convert_void_ptr_to_unsigned_char(const void *value)
{
    return *(unsigned char *)&value;
}
void *convert_unsigned_short_to_void_ptr(const unsigned short value)
{
    void *val = NULL;
    *(unsigned short *)&val = value;
    return val;
}
unsigned short convert_void_ptr_to_unsigned_short(const void *value)
{
    return *(unsigned short *)&value;
}