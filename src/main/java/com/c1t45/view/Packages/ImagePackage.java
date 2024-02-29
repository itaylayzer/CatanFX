package com.c1t45.view.Packages;

import javafx.scene.image.Image;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public class ImagePackage {
    final public String name;
    final public Paint color;
    final public Image image;

    public ImagePackage(String name, Image image) {
        this(name, Color.WHITE, image);
    }

    public ImagePackage(String name, Paint color, Image image) {
        this.name = name;
        this.color = color;
        this.image = image;
    }
}
