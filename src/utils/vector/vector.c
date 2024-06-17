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

    // for all elements
    while (--size >= 0)
    {
        // apply join function
        arr[size] = func(first[size], second[size]);
    }

    return arr;
}
signed char *vector_map(const signed char *first,
                        signed char size,
                        signed char (*func)(signed char))
{
    signed char *arr = calloc(size, sizeof(signed char));

    // for all elements
    while (--size >= 0)
    {
        // apply map function
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

    // if the loop stopped at index it means the condition is true for some element
    return size >= 0;
}
bool vector_all(const signed char *arr, signed char size, bool (*condition)(signed char))
{
    while (--size >= 0 && condition(arr[size]))
        ;

    // if the loop didnt stopped it means the condition is true for all elements
    return size < 0;
}
void vector_cpy(signed char *dest, signed char *src, signed char size)
{
    // for all elements
    while (--size >= 0)
    {
        // copy
        dest[size] = src[size];
    }
}

void vector_val(signed char *dest, signed char size, signed char val)
{

    // for all elements
    while (--size >= 0)
    {
        // set equals to val
        dest[size] = val;
    }
}
unsigned char vector_count(const signed char *dest, signed char size, signed char val)
{
    unsigned char count = 0;

    // for all elements
    while (--size >= 0)
    {
        // increase if the dest[size] equals to val
        count += dest[size] == val;
    }
    return count;
}
signed char vector_sum(signed char *arr, signed char size)
{
    signed char val;

    // for all elements
    while (--size >= 0)
    {
        val += arr[size];
    }
    return val;
}
signed char *vector_dup(signed char *arr, signed char size)
{
    signed char *new = malloc(sizeof(signed char) * size);

    // for all elements
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

    // for all elements
    for (offset = 0; offset < size; offset++)
    {
        // swap between current offset and random offset
        bswap(arr + offset, arr + brand(0, size));
    }
}
unsigned char vector_min_index(const signed char *arr, signed char size)
{
    unsigned char min_value = SIGNED_MAX_VALUE;
    unsigned char min_index = 0;

    // for all elements
    while (--size >= 0)
    {
        // change min value and min index
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
        // swap between index and size - index
        bswap(arr + offset, arr + size - 1 - offset);
    }
}

void print_vec(unsigned char *arr, signed char size)
{
    unsigned char offset;
    char *strings[] = {", %d", "%d"};

    printf("[");

    // for all elements
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
    // apply manipulate to first and second
    signed char *manipulated = manip(first, second, size);

    // apply conditions by any/all methods
    bool all = vector_all(manipulated, size, condition);
    bool any = vector_any(manipulated, size, condition);

    free(manipulated);

    // first bit is all second bit is any
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
        // find right index by the order
        fixed_index = order[size];

        // apply condition to find_index
        find_index = (fixed_index + 1) * condition(source[fixed_index]);
    }
    return find_index - 1;
}
unsigned char find_first_true_index(bool *arr, unsigned char size)
{
    unsigned char offset = 0;

    // while not true
    while (!arr[offset++])
        ;

    return offset - 1;
}
signed char *vector_upper_limit(signed char *source,
                                signed char *limits,
                                signed char size)
{
    // for all elements
    while (--size >= 0)
    {
        // apply the upper limit
        source[size] = bmin(source[size], limits[size]);
    }

    return source;
}