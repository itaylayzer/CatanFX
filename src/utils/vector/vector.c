#include "vector.h"

bool below_zero(signed char x)
{
    return x < 0;
}
bool above_equal_zero(signed char x)
{
    return x >= 0;
}
bool above_zero(signed char x)
{
    return x > 0;
}
signed char *vector_join(const signed char *first,
                         const signed char *second,
                         signed char size,
                         signed char (*func)(signed char first, signed char second))
{
    signed char *arr = calloc(size, sizeof(signed char));

    while (--size >= 0)
    {
        arr[size] = func(first[size], second[size]);
    }

    return arr;
}
signed char *vector_map(const signed char *first,
                        signed char size,
                        signed char (*func)(signed char))
{
    signed char *arr = calloc(size, sizeof(signed char));

    while (--size >= 0)
    {
        arr[size] = func(first[size]);
    }

    return arr;
}
signed char badd(signed char x, signed char y)
{
    return x + y;
}
signed char bminus(signed char x, signed char y)
{
    return x - y;
}
signed char bnegative(signed char x)
{
    return -x;
}
signed char *vector_add(const signed char *first,
                        const signed char *second,
                        signed char size)
{
    return vector_join(first, second, size, badd);
}

signed char *vector_sub(const signed char *first,
                        const signed char *second,
                        signed char size)
{
    return vector_join(first, second, size, bminus);
}
signed char *vector_neg(const signed char *arr,
                        signed char size)
{
    return vector_map(arr, size, bnegative);
}
bool vector_any(const signed char *arr, signed char size, bool (*condition)(signed char))
{
    while (--size >= 0 && !condition(arr[size]))
        ;

    return size >= 0;
}
bool vector_all(const signed char *arr, signed char size, bool (*condition)(signed char))
{
    while (--size >= 0 && condition(arr[size]))
        ;

    return size < 0;
}
void vector_cpy(signed char *dest, signed char *src, signed char size)
{
    while (--size >= 0)
    {
        dest[size] = src[size];
    }
}

void vector_val(signed char *dest, signed char size, signed char val)
{

    while (--size >= 0)
    {
        dest[size] = val;
    }
}
unsigned char vector_count(const signed char *dest, signed char size, signed char val)
{
    unsigned char count = 0;
    while (--size >= 0)
    {
        count += dest[size] == val;
    }
    return count;
}
signed char vector_sum(signed char *arr, signed char size)
{
    unsigned char val;

    while (--size >= 0)
    {
        val += arr[size];
    }
    return val;
}
signed char *vector_dup(signed char *arr, signed char size)
{
    signed char *new = malloc(sizeof(signed char) * size);
    while (--size >= 0)
    {
        new[size] = arr[size];
    }
    return new;
}
void bswap(signed char *x, signed char *y)
{
    signed char temp = *y;
    *y = *x;
    *x = temp;
}
void vector_shuffle(signed char *arr, unsigned char size)
{
    unsigned char offset;
    for (offset = 0; offset < size; offset++)
    {
        bswap(arr + offset, arr + brand(0, size));
    }
}
unsigned char vector_min_index(const signed char *arr, signed char size)
{
    unsigned char min_value = SIGNED_MAX_VALUE;
    unsigned char min_index = 0;
    while (--size >= 0)
    {
        (arr[size] < min_value) &&
            (min_value = arr[size],
             min_index = size);
    }
    return min_index;
}
void vector_reverse(signed char *arr, unsigned char size)
{
    unsigned char offset;
    signed char temp;

    for (offset = 0; offset * 2 < size; offset++)
    {
        temp = arr[offset];
        arr[offset] = arr[size - 1 - offset];
        arr[size - 1 - offset] = temp;
    }
}

void print_vec(unsigned char *arr, signed char size)
{
    unsigned char offset;
    char *strings[] = {", %d", "%d"};

    printf("[");

    for (offset = 0; offset < size; offset++)
    {
        printf(strings[!offset], arr[offset]);
    }

    printf("]\n");
}

unsigned char vector_manip_condition(const signed char *first,
                                     const signed char *second,
                                     unsigned char size,
                                     signed char *(*manip)(const signed char *, const signed char *, signed char),
                                     bool (*condition)(signed char))
{
    signed char *manipulated = manip(first, second, size);
    bool all = vector_all(manipulated, size, condition);
    bool any = vector_any(manipulated, size, condition);
    free(manipulated);
    return all + (any << 1);
}
signed char vector_order_find_last(const signed char *source,
                                   const signed char *order,
                                   signed char size,
                                   bool (*condition)(signed char))
{
    signed char find_index = 0, fixed_index;
    while (--size >= 0 && !find_index)
    {
        fixed_index = order[size];
        find_index = (fixed_index + 1) * condition(source[fixed_index]);
    }
    return find_index - 1;
}
unsigned char find_first_true_index(bool *arr, unsigned char size)
{
    unsigned char offset = 0;
    while (!arr[offset])
        offset++;
    return offset;
}
signed char *vector_upper_limit(signed char *source, signed char *limits, signed char size)
{
    while (--size >= 0)
    {
        source[size] = bmin(source[size], limits[size]);
    }

    return source;
}