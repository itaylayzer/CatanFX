package com.c1t45.view.Utils;

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
}
