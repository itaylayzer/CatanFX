#ifndef _INC_VEC_UTILS

#define _INC_VEC_UTILS

#include <stdbool.h>
#include <stdlib.h>

#include "../../utils/math/compare.h"
#include "../../utils/math/rand.h"

#define SIGNED_MAX_VALUE 127

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
signed char vec_sum(signed char *, signed char size);
signed char *vec_dup(signed char *, signed char size);
void vec_shuffle(signed char *, unsigned char size);

unsigned char vector_min_index(const signed char *, signed char size);
#endif