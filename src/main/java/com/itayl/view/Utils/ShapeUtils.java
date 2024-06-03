package com.itayl.view.Utils;

public class ShapeUtils {

    public static double[] hexagon(double x, double y, double size, double v) {
        return new double[] {
                x + v * size, y,
                x, y + size * 0.5,
                x, y + size * 1.5,
                x + v * size, y + size * 2,
                x + 2 * v * size, y + size * 1.5,
                x + 2 * v * size, y + size * 0.5,
        };
    }
}
