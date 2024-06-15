#ifndef _INC_VEC_UTILS

#define _INC_VEC_UTILS

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../utils/math/compare.h"
#include "../../utils/math/rand.h"

#define SIGNED_MAX_VALUE 127

bool below_zero(signed char x);
bool above_equal_zero(signed char x);
bool above_zero(signed char x);
signed char *vector_join(const signed char *first,
                         const signed char *second,
                         signed char size,
                         signed char (*func)(signed char first, signed char second));
signed char *vector_map(const signed char *, signed char size, signed char (*func)(signed char));
signed char *vector_add(const signed char *first, const signed char *second, signed char size);
signed char *vector_sub(const signed char *first, const signed char *second, signed char size);
signed char *vector_neg(const signed char *, signed char size);
bool vector_any(const signed char *, signed char size, bool (*condition)(signed char));
bool vector_all(const signed char *, signed char size, bool (*condition)(signed char));
void vector_cpy(signed char *dest, signed char *src, signed char size);
void vector_val(signed char *dest, signed char size, signed char val);
unsigned char vector_count(const signed char *, signed char size, signed char val);
signed char vector_sum(signed char *, signed char size);
signed char *vector_dup(signed char *, signed char size);
void vector_shuffle(signed char *, unsigned char size);

unsigned char vector_min_index(const signed char *, signed char size);
void vector_reverse(signed char *, unsigned char size);

void print_vec(unsigned char *arr, signed char size);
unsigned char vector_manip_condition(const signed char *first,
                                     const signed char *second,
                                     unsigned char size,
                                     signed char *(*manip)(const signed char *, const signed char *, signed char),
                                     bool (*condition)(signed char));
signed char vector_order_find_last(const signed char *source,
                                   const signed char *order,
                                   signed char size,
                                   bool (*condition)(signed char));
#endif