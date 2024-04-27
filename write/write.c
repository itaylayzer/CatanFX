#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/math/compare.h"

#define byte char

const byte settlementsRowsInitials[] = {0, 3, 7, 11, 16, 21, 27, 33, 38, 43, 47, 51, 54};
const byte areaRowsInitials[] = {0, 3, 7, 12, 16, 19};

const byte areasRowAmount[] = {3, 4, 5, 4, 3};
const byte settlementsPerInitialsRowHexes[][6] = {
    {0, 3, 4, 7, 8, 12},
    {7, 11, 12, 16, 17, 22},
    {16, 21, 22, 27, 28, 33},
    {28, 33, 34, 38, 39, 43},
    {39, 43, 44, 47, 48, 51}};

byte getSettlementRow(byte dot)
{
    byte offset;
    for (offset = 1; offset < sizeof(settlementsRowsInitials) - 1 && settlementsRowsInitials[offset] <= dot; offset++)
        ;
    return offset - 1;
}

byte *adjacentAreaVertecies(byte hexIndex)
{
    byte currentRowFirstHex = 0;
    byte index, offset;

    for (index = 0; index < 5 && currentRowFirstHex + areasRowAmount[index] <= hexIndex; index++)
        currentRowFirstHex += areasRowAmount[index];

    index = bmin(index, 4);
    offset = hexIndex - currentRowFirstHex;
    byte *dots = malloc(sizeof(byte) * 6);

    dots[0] = settlementsPerInitialsRowHexes[index][0];
    dots[1] = settlementsPerInitialsRowHexes[index][1];
    dots[2] = settlementsPerInitialsRowHexes[index][2];
    dots[3] = settlementsPerInitialsRowHexes[index][3];
    dots[4] = settlementsPerInitialsRowHexes[index][4];
    dots[5] = settlementsPerInitialsRowHexes[index][5];

    while (offset-- > 0)
        for (index = 0; index < 6; index++)
            dots[index]++;

    return dots;
}

byte modify(byte offset, byte addon, byte axis)
{
    return bmin(
        settlementsRowsInitials[axis + 1] - 1,
        bmax(settlementsRowsInitials[axis], settlementsRowsInitials[axis] + offset + addon));
}

void removeDuplicatesAndValue(byte *dots, byte *size, byte dot)
{
    byte seen[256] = {0};
    byte index = 0;

    for (byte i = 0; i < *size; i++)
        if (seen[dots[i]] == 0 && dots[i] != dot)
        {
            dots[index++] = dots[i];
            seen[dots[i]] = 1;
        }

    *size = index;
}

byte *adjacentVertecies(byte dot, byte *size)
{
    const byte vertexRow = getSettlementRow(dot);

    const byte offset = dot - settlementsRowsInitials[vertexRow];

    const byte above = bmax(vertexRow - 1, 0);
    const byte below = bmin(vertexRow + 1, sizeof(settlementsRowsInitials) - 2);

    bool isEvan = vertexRow % 2 == 0;
    byte *dots = malloc(sizeof(byte) * 3);

    const char isAbove5 = vertexRow > 5;

    dots[0] = (modify(offset, 0, above));
    dots[1] = (modify(offset, 0 + -(isAbove5 && isEvan), below));
    dots[2] = (modify(offset, isEvan ? 1 + -isAbove5 : -1 + 2 * +isAbove5, isEvan ? below : above));

    *size = 3;
    removeDuplicatesAndValue(dots, size, dot);

    dots = realloc(dots, *size * sizeof(byte));
    return dots;
}

int main()
{
    FILE *file;
    file = fopen("edges.byte", "wb");

    if (!file)
        exit(EXIT_FAILURE);

    const byte AREAS = 19;
    const byte DOTS = 54;

    char *dots, size;
    byte offset, index;

    for (offset = 0; offset < AREAS; offset++)
    {
        dots = adjacentAreaVertecies(offset);
        for (index = 0; index < 6; index++)
        {
            byte dot = dots[index] + AREAS;
            byte elements[4] = {offset, dot, dot, offset};
            fwrite(elements, sizeof(byte), 4, file);
        }
        free(dots);
    }

    for (offset = 0; offset < DOTS; offset++)
    {
        size = 3;
        dots = adjacentVertecies(offset, &size);

        for (index = 0; index < size; index++)
        {
            byte org = offset + AREAS;
            byte dest = dots[index] + AREAS;
            byte elements[2] = {org, dest};
            fwrite(elements, sizeof(byte), 2, file);
        }
        free(dots);
    }

    fclose(file);
    return 0;
}