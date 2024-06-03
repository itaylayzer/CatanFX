package com.itayl.view.Packages;

import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public class NotificationPackage {
    final public String message;
    final public Paint color;

    public NotificationPackage(String message) {
        this(message, Color.BLACK);
    }

    public NotificationPackage(String message, Paint color) {
        this.message = message;
        this.color = color;
    }

}
