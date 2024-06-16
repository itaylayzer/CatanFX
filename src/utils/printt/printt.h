#ifndef _INC_PRINTT

#define _INC_PRINTT

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//----------------------------------------------------------------------------------------
//                                 Print with Time
//                                 ---------------
//
// General      : The function prints a formatted message with Timestamp
//
// Parameters   :
//          format     - String (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void printt(const char *format, ...);

//----------------------------------------------------------------------------------------
//                                 Put with Time
//                                 -------------
//
// General      : The functions prints a message with Timestamp
//
// Parameters   :
//          message    - String (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void putts(const char *message);
#endif