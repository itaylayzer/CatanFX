#include "constants.h"
const signed char store[TOTAL_STORE][TOTAL_MATERIALS] = {
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 2, 0, 3},
    {0, 1, 1, 0, 1}};

const float probs[11] = {2.78, 5.56, 8.33, 11.11, 13.89, 16.67,
                         13.89, 11.11, 8.83, 5.56, 2.78};