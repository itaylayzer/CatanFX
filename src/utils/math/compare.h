#ifndef _INC_MATH_COMPARE

#define _INC_MATH_COMPARE

//----------------------------------------------------------------------------------------
//                                 Byte Sign
//                                 ---------
//
// General      : The function calculates the byte value sign.
//
// Parameters   :
//          x          - Byte Value (In)
//
// Return Value : -1 if the Value is Negative, otherwise 1.
//
//----------------------------------------------------------------------------------------
signed char bsign(signed char x);

//----------------------------------------------------------------------------------------
//                                 Byte Absolute
//                                 -------------
//
// General      : The function calculates the absolute value of the byte parameter.
//
// Parameters   :
//          x          - Byte Value (In)
//
// Return Value : Absolute value of x.
//
//----------------------------------------------------------------------------------------
unsigned char babs(signed char x);

//----------------------------------------------------------------------------------------
//                                 Byte Min
//                                 --------
//
// General      : The function input 2 byte values and return the minimum value of them.
//
// Parameters   :
//          x          - Byte Value (In)
//          y          - Byte Value (In)
//
// Return Value : The minimum value of x and y.
//
//----------------------------------------------------------------------------------------
signed char bmin(signed char x, signed char y);

//----------------------------------------------------------------------------------------
//                                 Byte Max
//                                 --------
//
// General      : The function input 2 byte values and return the maaximum value of them.
//
// Parameters   :
//          x          - Byte Value (In)
//          y          - Byte Value (In)
//
// Return Value : The maximum value of x and y.
//
//----------------------------------------------------------------------------------------
signed char bmax(signed char x, signed char y);

#endif