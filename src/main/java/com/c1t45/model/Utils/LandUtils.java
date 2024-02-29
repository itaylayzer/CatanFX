package com.c1t45.model.Utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class LandUtils {

    public static List<Byte> vrtexToIndexes() {
        return new ArrayList<>();
    }

    private static final byte[][] startingDots = {
            { 0, 3, 4, 7, 8, 12 },
            { 7, 11, 12, 16, 17, 22 },
            { 16, 21, 22, 27, 28, 33 },
            { 28, 33, 34, 38, 39, 43 },
            { 39, 43, 44, 47, 48, 51 }
    };

    private static final byte[] firstHexes = { 3, 4, 5, 4, 3 };

    public static byte[] indexToVertecis(byte hexIndex) {
        byte currentRowFirstHex = 0;
        byte index, offset;

        for (index = 0; index < 5 && (currentRowFirstHex + firstHexes[index]) <= hexIndex; index++) {
            currentRowFirstHex += firstHexes[index];
        }

        index = (byte) Math.min(index, 4);

        offset = (byte) (hexIndex - currentRowFirstHex);
        byte[] dots = Arrays.copyOf(startingDots[index], startingDots[index].length);

        while (offset-- > 0) {
            dots = BytesUtils.add(dots, new byte[] { 1, 1, 1, 1, 1, 1 });
        }

        return dots;
    }

}
