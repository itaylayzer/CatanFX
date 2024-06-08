#include "constants.h"
const signed char store[TOTAL_STORE][TOTAL_MATERIALS] = {
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 2, 0, 3},
    {0, 1, 1, 0, 1}};

const float probs[11] = {2.78, 5.56, 8.33, 11.11, 13.89, 0,
                         13.89, 11.11, 8.83, 5.56, 2.78};
unsigned char harbors[HARBOR_COUNT * 2] = {0, 3, 1, 5, 10, 15, 26, 32, 42, 46, 49, 52, 47, 51, 33, 38, 11, 16};