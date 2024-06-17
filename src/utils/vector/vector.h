#ifndef _INC_VEC_UTILS

#define _INC_VEC_UTILS

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../math/compare.h"
#include "../math/rand.h"

#define SIGNED_MAX_VALUE 127

//----------------------------------------------------------------------------------------
//                                 below zero
//                                 ----------
//
// General      : The functions checks if a byte value is below zero
//
// Parameters   :
//          x          - Byte Value (In)
//
// Return Value : True if x is below zero, otherwise False
//
//----------------------------------------------------------------------------------------
bool below_zero(signed char x);

//----------------------------------------------------------------------------------------
//                                 above equal zero
//                                 ----------------
//
// General      : The functions checks if a byte value is above zero or equal to zero
//
// Parameters   :
//          x          - Byte Value (In)
//
// Return Value : True if x is above or equal to zero, otherwise False
//
//----------------------------------------------------------------------------------------
bool above_equal_zero(signed char x);

//----------------------------------------------------------------------------------------
//                                 above equal zero
//                                 ----------------
//
// General      : The functions checks if a byte value is above zero
//
// Parameters   :
//          x          - Byte Value (In)
//
// Return Value : True if x is above zero, otherwise False
//
//----------------------------------------------------------------------------------------
bool above_zero(signed char x);

//----------------------------------------------------------------------------------------
//                                 vector join
//                                 -----------
//
// General      : The function takes 2 vectors and a join function,
//                  and joins them into 1 vector
//
// Parameters   :
//          first      - Vector (In)
//          second     - Vector (In)
//          size       - Vectors Size (In)
//          func       - Join Function (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_join(const signed char *first,
                         const signed char *second,
                         signed char size,
                         signed char (*func)(signed char first, signed char second));

//----------------------------------------------------------------------------------------
//                                 vector map
//                                 ----------
//
// General      : The functions takes a vector and a map function,
//                 and apply the map to every element to a new vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//          func       - Map Function (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_map(const signed char *arr,
                        signed char size,
                        signed char (*func)(signed char));

//----------------------------------------------------------------------------------------
//                                 vector add
//                                 ----------
//
// General      : The function takes 2 vectors and adds them to another vector
//
// Parameters   :
//          first      - Vector (In)
//          second     - Vector (In)
//          size       - Vectors Size (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_add(const signed char *first, const signed char *second, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector sub
//                                 ----------
//
// General      : The function takes 2 vectors and subs them to another vector
//
// Parameters   :
//          first      - Vector (In)
//          second     - Vector (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_sub(const signed char *first, const signed char *second, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector neg
//                                 ----------
//
// General      : The function takes a vector and return the negative vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_neg(const signed char *arr, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector any
//                                 ----------
//
// General      : The function takes a vector and a condition function
//                 and check if the condition apply to at least one of the elements
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//          condition  - Condition Function (In)
//
// Return Value : True if the condition apply to at least one of the elements,
//                  otherwise False
//
//----------------------------------------------------------------------------------------
bool vector_any(const signed char *arr, signed char size, bool (*condition)(signed char));

//----------------------------------------------------------------------------------------
//                                 vector all
//                                 ----------
//
// General      : The function takes a vector and a condition function
//                 and check if the condition apply to all the elements
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//          condition  - Condition Function (In)
//
// Return Value : True if the condition apply to all of the elements,
//                  otherwise False
//
//----------------------------------------------------------------------------------------
bool vector_all(const signed char *arr, signed char size, bool (*condition)(signed char));

//----------------------------------------------------------------------------------------
//                                 vector copy
//                                 -----------
//
// General      : The function copies an entire vector to another vector
//
// Parameters   :
//          dest       - Destiny Vector (Out)
//          src        - Source Vector (In)
//          size       - Vectors Size (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void vector_cpy(signed char *dest, signed char *src, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector value
//                                 ------------
//
// General      : The function copies a value to the entire vector
//
// Parameters   :
//          dest       - Destiny Vector (Out)
//          size       - Vectors Size (In)
//          val        - Value (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void vector_val(signed char *dest, signed char size, signed char val);

//----------------------------------------------------------------------------------------
//                                 vector count
//                                 ------------
//
// General      : The function count how many of a value exsist in a vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//          val        - Value (In)
//
// Return Value : Counts number
//
//----------------------------------------------------------------------------------------
unsigned char vector_count(const signed char *arr, signed char size, signed char val);

//----------------------------------------------------------------------------------------
//                                 vector sum
//                                 ----------
//
// General      : The functions sums up a vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Summed number
//
//----------------------------------------------------------------------------------------
signed char vector_sum(signed char *arr, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector duplicate
//                                 ----------------
//
// General      : The function duplicates a vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_dup(signed char *arr, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector shuffle
//                                 --------------
//
// General      : The functions shuffles a vectors elements
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void vector_shuffle(signed char *arr, unsigned char size);

//----------------------------------------------------------------------------------------
//                                 vector min val index
//                                 --------------------
//
// General      : The function finds the min val index in a vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Index number
//
//----------------------------------------------------------------------------------------
unsigned char vector_min_index(const signed char *arr, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector reverse
//                                 --------------
//
// General      : The functions reverse a vector
//
// Parameters   :
//          arr        - Vector (In)
//          size       - Vector Size (In)
//
// Return Value : None.
//
//----------------------------------------------------------------------------------------
void vector_reverse(signed char *arr, unsigned char size);

void print_vec(unsigned char *arr, signed char size);

//----------------------------------------------------------------------------------------
//                                 vector manipulate condition
//                                 ---------------------------
//
// General      : The functions joins 2 vectors and apply condition to it,
//                  in both all and any methods
//
// Parameters   :
//          first      - Vector (In)
//          second     - Vector (in)
//          size       - Vectors Size (In)
//          manip      - Join Function (In)
//          condition  - Condition Function (In)
//
// Return Value : Bitmap value where the first bit is all and second bit is any
//
//----------------------------------------------------------------------------------------
unsigned char vector_manip_condition(const signed char *first,
                                     const signed char *second,
                                     unsigned char size,
                                     signed char *(*manip)(const signed char *, const signed char *, signed char),
                                     bool (*condition)(signed char));

//----------------------------------------------------------------------------------------
//                                 vector order find last
//                                 ----------------------
//
// General      : The functions find last element that apply to a condition
//                  in a specific order
//
// Parameters   :
//          source     - Vector (In)
//          order      - Order Vector (In)
//          size       - Vectors Size (In)
//          condition  - Condition Function (In)
//
// Return Value : Index of the value if found, otherwise -1
//
//----------------------------------------------------------------------------------------
signed char vector_order_find_last(const signed char *source,
                                   const signed char *order,
                                   signed char size,
                                   bool (*condition)(signed char));

//----------------------------------------------------------------------------------------
//                                 find first true index
//                                 ---------------------
//
// General      : The functions find first true index in a booleans vector
//
// Parameters   :
//          arr        - Booleans Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Index of the element
//
//----------------------------------------------------------------------------------------
unsigned char find_first_true_index(bool *arr, unsigned char size);

//----------------------------------------------------------------------------------------
//                                 vector upper limit
//                                 ------------------
//
// General      : The functions apply upper limit to a vector
//
// Parameters   :
//          source     - Vector (In/Out)
//          limits     - Limit Vector (In)
//          size       - Vector Size (In)
//
// Return Value : Source Vector
//
//----------------------------------------------------------------------------------------
signed char *vector_upper_limit(signed char *source, signed char *limits, signed char size);
#endif