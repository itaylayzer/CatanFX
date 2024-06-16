#ifndef _INC_MATH_RAND

#define _INC_MATH_RAND
#include <stdlib.h>

#include "./compare.h"

//----------------------------------------------------------------------------------------
//                                 Byte Random
//                                 -----------
//
// General      : The function calculates random value between an inputed range
//
// Parameters   :
//          min        - Minimum range (Inclusive) (In)
//          max        - Maximum Range (Not Inclusive) (In)
//
// Return Value : Random value between min and max - 1.
//
//----------------------------------------------------------------------------------------
unsigned char brand(unsigned char min, unsigned char max);

#endif