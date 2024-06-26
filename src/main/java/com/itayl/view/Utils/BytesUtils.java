package com.itayl.view.Utils;

import java.util.ArrayList;
import java.util.List;

import com.itayl.view.Interfaces.Operator;
import com.itayl.view.Interfaces.Predicate;

public class BytesUtils {

    public static byte fromBoolean(boolean condition) {
        return (byte) (condition ? 1 : 0);
    }

    public static byte[] duplicate(byte[] source) {
        int index;
        byte[] dest = new byte[source.length];

        for (index = 0; index < source.length; index++) {
            dest[index] = source[index];
        }
        return dest;
    }

    private static byte[] operator(byte[] dest, byte[] array, Operator<Byte> operator) {
        int index, length = Math.min(dest.length, array.length);

        for (index = 0; index < length; index++)
            dest[index] = operator.operate(dest[index], array[index]);
        return dest;
    }

    public static byte[] add(byte[] dest, byte[] array) {

        byte[] result = operator(dest, array, new Operator<Byte>() {
            @Override
            public Byte operate(Byte dest, Byte source) {
                byte a = dest;
                byte b = source;
                return (byte) (a + b);
            }
        });
        return result;
    }

    public static byte[] minus(byte[] dest, byte[] array) {

        byte[] result = operator(dest, array, new Operator<Byte>() {
            @Override
            public Byte operate(Byte dest, Byte source) {
                byte a = dest;
                byte b = source;
                return (byte) (a - b);
            }
        });
        return result;
    }

    public static List<Byte> filter(byte[] source, Predicate<Byte> predicate) {
        List<Byte> bytes = new ArrayList<>();
        int index;
        for (index = 0; index < source.length; index++) {
            if (predicate.condition(source[index], index)) {
                bytes.add(source[index]);
            }
        }
        return bytes;
    }

    public static byte[] listToArray(List<Byte> bytes) {
        byte[] array = new byte[bytes.size()];
        int index = 0;
        for (Byte element : bytes) {
            array[index++] = element;
        }
        return array;
    }

    public static boolean bit(byte num, byte bit) {
        return ((num >> bit) & 0x01) == 1;
    }

    public static String bytesString(byte[] materials) {
        String st = "[";
        for (int index = 0; index < materials.length; index++) {
            st += String.format(index == 0 ? "%d" : ", %d", materials[index]);
        }
        st += "]";
        return st;
    }

    public static String singleString(byte _byte) {
        return String.format("%8s", Integer.toBinaryString(_byte & 0xFF)).replace(' ', '0');
    }

    public static byte find(byte[] arr, byte val) {
        byte offset;
        for (offset = (byte) 0; offset < arr.length; offset++) {
            if (arr[offset] == val)
                return offset;
        }
        return (byte) -1;
    }

    public static byte max(byte a, byte b) {
        return a > b ? a : b;
    }
}
