#ifndef _INC_CONVERT
#define _INC_CONVERT
#include <stdlib.h>

//----------------------------------------------------------------------------------------
//                                 convert unsigned char to void ptr
//                                 ---------------------------------
//
// General      : Convert unsigned char value to void* value.
//
// Parameters   :
//          value   - Value (In)
//
// Return Value : Value (8 bytes)
//
//----------------------------------------------------------------------------------------
void *convert_unsigned_char_to_void_ptr(const unsigned char value);

//----------------------------------------------------------------------------------------
//                                 convert void ptr to unsigned char
//                                 ---------------------------------
//
// General      : Convert void* value to unsigned char value.
//
// Parameters   :
//          value   - Value (In)
//
// Return Value : Value (byte)
//
//----------------------------------------------------------------------------------------
unsigned char convert_void_ptr_to_unsigned_char(const void *value);

//----------------------------------------------------------------------------------------
//                                 convert unsigned short to void ptr
//                                 ----------------------------------
//
// General      : Convert unsigned short value to void* value.
//
// Parameters   :
//          value   - Value (In)
//
// Return Value : Value (8 bytes)
//
//----------------------------------------------------------------------------------------
void *convert_unsigned_short_to_void_ptr(const unsigned short value);

//----------------------------------------------------------------------------------------
//                                 convert void ptr to unsigned short
//                                 ----------------------------------
//
// General      : Convert void* value to unsigned short value.
//
// Parameters   :
//          value   - Value (In)
//
// Return Value : Value (2 bytes)
//
//----------------------------------------------------------------------------------------
unsigned short convert_void_ptr_to_unsigned_short(const void *value);
#endif