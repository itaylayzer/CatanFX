package com.itayl.view.Utils;

public class DoubleUtils {
    public static int roundToTwoDecimals(double num) {
        return (int) Math.round(num * 100) / 100;
    }
}
