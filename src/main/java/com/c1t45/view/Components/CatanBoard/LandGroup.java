package com.c1t45.view.Components.CatanBoard;

import javafx.scene.Group;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Polygon;
import javafx.scene.shape.StrokeType;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;

import java.util.Arrays;

import com.c1t45.view.Packages.ImagePackage;
import com.c1t45.view.Utils.ImageUtils;
import com.c1t45.view.Utils.ShapeUtils;

public class LandGroup extends Group {

    private Polygon tile;
    private Text numberText;
    private ImageView imageView;

    private ImagePackage info;
    private Byte number;
    private final double x, y, size, v;

    private final byte fontNum = 60;
    private final String fontString = "Segoe UI";
    private final double imageScale = 0.2;

    public LandGroup(ImagePackage info, Byte number, double x, double y, double size, double v) {

        // land info
        this.number = number;
        this.info = info;
        this.x = x;
        this.y = y;
        this.size = size;
        this.v = v;

        // land components
        this.tile = new Polygon();
        this.numberText = new Text();
        this.imageView = new ImageView();

        final double centerX = x + v * size / 2;
        final double centerY = y + size / 2;

        initTile();
        tile.setFill(Paint.valueOf("#333333"));
        String numberString = "7";

        if (info != null) {
            numberString = this.number.toString();
            tile.setFill(((Color) this.info.color).deriveColor(0.0, 1.0, 0.8, 1.0));
            initImage(centerX, centerY);
        }

        if (info != null) {
            Color brighterColor = ((Color) this.info.color).deriveColor(0.0, 0.8, 2, 1.6);
            initText(numberString, centerX, centerY, brighterColor);
            numberText.setFill(brighterColor);
            numberText.setStroke(brighterColor);
        } else {
            initTextCenter(numberString, centerX, centerY);
        }

        super.getChildren().add(this.tile);
        super.getChildren().add(this.numberText);
        if (this.number != 7) {
            super.getChildren().add(this.imageView);
        }

    }

    private double[] hexagon() {
        return ShapeUtils.hexagon(x, y, size, v);
    }

    private void initTile() {

        double[] doubleHex = hexagon();
        Double[] DoubledHex = Arrays.stream(doubleHex).boxed().toArray(Double[]::new);

        tile.getPoints().addAll(DoubledHex);
        tile.setFill(Color.WHITE);

        tile.setStrokeWidth(3);
        tile.setStrokeType(StrokeType.CENTERED);

        tile.setStroke(Paint.valueOf("#101010"));
    }

    private void initImage(
            final double centerX,
            final double centerY) {
        imageView.setImage(ImageUtils.colorChange(info.image));
        imageView.setLayoutX(centerX - 110);
        imageView.setLayoutY(centerY - 130);
        imageView.setScaleX(imageScale * 0.7);
        imageView.setScaleY(imageScale * 0.7);
        imageView.setOpacity(0.5);
    }

    private void initText(final String numberString, final double centerX, final double centerY, Color shadowColor) {

        numberText = new Text(0, fontNum / 4, numberString);
        numberText.setFill(Color.WHITESMOKE);
        numberText.setStrokeWidth(2);
        numberText.setStroke(Color.WHITE);
        numberText.setOpacity(0.7);
        numberText.setFont(Font.font(fontString, FontWeight.BOLD, FontPosture.REGULAR, fontNum / 2));

        numberText.setLayoutX(centerX - (numberString.length() * fontNum / 6));
        numberText.setLayoutY(centerY + fontNum / 3);
        numberText.setTextAlignment(TextAlignment.CENTER);
    }

    private void initTextCenter(
            final String numberString,
            final double centerX,
            final double centerY) {

        numberText = new Text(0, fontNum, numberString);
        numberText.setFill(Color.WHITESMOKE);
        numberText.setStrokeWidth(2);
        numberText.setStroke(Color.WHITE);
        numberText.setOpacity(1);
        numberText.setFont(Font.font(fontString, FontWeight.BOLD, FontPosture.REGULAR, fontNum));

        DropShadow shadow = new DropShadow();
        shadow.setOffsetX(0);
        shadow.setOffsetY(0);
        shadow.setColor(Color.WHITE);
        shadow.setRadius(3);
        shadow.setSpread(1);
        numberText.setEffect(shadow);

        numberText.setLayoutX(centerX + 24 - (numberString.length() * fontNum / 3));
        numberText.setLayoutY(centerY - fontNum / 5);
        numberText.setTextAlignment(TextAlignment.CENTER);
    }

    public void toFront() {
        super.toFront();
        tile.toFront();
        imageView.toFront();
        numberText.toFront();
    }

}
