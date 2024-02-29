package com.c1t45.view.CatanBoard;

import com.c1t45.model.Timeout;
import com.c1t45.model.Interfaces.Action;
import com.c1t45.view.Packages.NotificationPackage;

import javafx.animation.Interpolator;
import javafx.animation.TranslateTransition;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;

import javafx.util.Duration;

public class NotificationGroup extends Group {
    private static final double width = 170;
    private static final Interpolator intIn = Interpolator.SPLINE(0, .5, 0, 1);
    private static final Interpolator intOut = Interpolator.SPLINE(1, 0, 1, 1);

    public NotificationGroup(NotificationPackage pack, Action<NotificationGroup> disconnect) {

        setTranslateX(-width);
        translateIn();

        Rectangle rectangle = new Rectangle(width, 25);
        rectangle.setFill(Paint.valueOf("#1a1a1a"));
        rectangle.setArcHeight(0);
        rectangle.setArcWidth(0);

        Rectangle colorRect = new Rectangle(8, 25);
        colorRect.setFill(pack.color);

        Label label = new Label(pack.message);
        label.setFont(Font.font("Segoe", FontWeight.LIGHT, FontPosture.ITALIC, 18));
        label.setOpacity(0.7);
        label.setTextFill(Color.WHITE);
        label.setPrefWidth(width);
        label.setAlignment(Pos.CENTER);

        getChildren().add(rectangle);
        getChildren().add(colorRect);
        getChildren().add(label);

        Timeout.timeout(() -> {
            this.translateOut(disconnect);
        }, 5000);
    }

    private void translateIn() {
        TranslateTransition translateX = new TranslateTransition(Duration.seconds(0.5), this);
        translateX.setFromX(-width);
        translateX.setToX(0);
        translateX.setInterpolator(intIn);
        translateX.play();
    }

    private void translateOut(Action<NotificationGroup> disconnect) {
        TranslateTransition translateX = new TranslateTransition(Duration.seconds(0.3), this);
        translateX.setFromX(0);
        translateX.setToX(-width);
        translateX.setInterpolator(intOut);
        translateX.setOnFinished((event) -> {
            disconnect.action(this);
        });
        translateX.play();
    }
}
