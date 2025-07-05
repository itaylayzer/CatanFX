#ifndef _INC_CONST
#define _INC_CONST
#include "./types.h"

//----------------------------------------------------------------------------------------
//                                 constants
//                                 ---------
//
// General      : stores constants values.
//
//----------------------------------------------------------------------------------------

extern const signed char store[TOTAL_STORE][TOTAL_MATERIALS];
extern const float probs[11];
extern unsigned char harbors[HARBOR_COUNT * 2];
#endif