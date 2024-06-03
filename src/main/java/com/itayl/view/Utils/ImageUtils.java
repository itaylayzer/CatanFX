package com.itayl.view.Utils;

import javafx.scene.image.Image;
import javafx.scene.image.PixelReader;
import javafx.scene.image.PixelWriter;
import javafx.scene.paint.Color;

public class ImageUtils {
    public static Image colorChange(Image originalImage) {
        return colorChange(originalImage, Color.BLACK, Color.WHITE);

    }

    public static Image colorChange(Image originalImage, Color to) {
        return colorChange(originalImage, Color.BLACK, to);
    }

    public static Image colorChange(Image originalImage, Color from, Color to) {
        int width = (int) originalImage.getWidth();
        int height = (int) originalImage.getHeight();
        javafx.scene.image.WritableImage writableImage = new javafx.scene.image.WritableImage(width, height);

        PixelReader pixelReader = originalImage.getPixelReader();
        PixelWriter pixelWriter = writableImage.getPixelWriter();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color color = pixelReader.getColor(x, y);

                if (color.equals(from)) {
                    color = to;
                }

                pixelWriter.setColor(x, y, color);
            }
        }

        return writableImage;
    }

    public static Color colorOpacity(Color color, double colorOpacity) {
        double red = color.getRed() * colorOpacity;
        double green = color.getGreen() * colorOpacity;
        double blue = color.getBlue() * colorOpacity;

        // Ensure values are within the valid range [0, 1]
        red = Math.min(Math.max(red, 0.0), 1.0);
        green = Math.min(Math.max(green, 0.0), 1.0);
        blue = Math.min(Math.max(blue, 0.0), 1.0);

        return new Color(red, green, blue, color.getOpacity());

    }
}
