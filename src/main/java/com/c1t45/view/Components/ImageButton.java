package com.c1t45.view.Components;

import javafx.animation.FadeTransition;
import javafx.animation.Interpolator;
import javafx.animation.ScaleTransition;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;
import javafx.util.Duration;

public class ImageButton extends Group {
    protected Rectangle mainRectangle;
    protected Rectangle imageRectangle;
    protected ImageView imageView;
    private Label label;
    private Tooltip tooltip;

    private EventHandler<MouseEvent> onMouseEntered;
    private EventHandler<MouseEvent> onMouseExited;
    private EventHandler<Event> onDisabled;
    private EventHandler<Event> onEnabled;

    private double width;
    private double height;
    private double gap;
    private double fontSize;
    private double tooltipFontSize;

    private boolean scaleOnHover;
    private boolean buttonDisabled;

    public ImageButton(double width, double height, double gap) {
        super();

        this.width = width;
        this.height = height;
        this.gap = gap;
        this.fontSize = 30;
        this.tooltipFontSize = 14;
        this.scaleOnHover = true;

        mainRectangle = new Rectangle();
        imageRectangle = new Rectangle();
        imageView = new ImageView();
        label = null;
        tooltip = null;

        initialize();

        super.getChildren().add(mainRectangle);
        super.getChildren().add(imageRectangle);
        super.getChildren().add(imageView);

        super.setOnMouseEntered((event) -> {

            if (tooltip != null) {
                Point2D point = locationTooltip();
                tooltip.show(super.getScene().getWindow(), point.getX(), point.getY());
            }

            if (onMouseEntered != null)
                onMouseEntered.handle(event);

            if (scaleOnHover)
                scale(0.9);
        });
        super.setOnMouseExited((event) -> {
            if (tooltip != null) {
                tooltip.hide();
            }

            if (onMouseExited != null) {
                onMouseExited.handle(event);
            }

            if (scaleOnHover)
                scale(1);
        });
    }

    private void initialize() {
        mainRectangle.setWidth(width);
        mainRectangle.setHeight(height);
        mainRectangle.setFill(Color.BLACK);
        mainRectangle.setArcWidth(20);
        mainRectangle.setArcHeight(20);

        imageRectangle.setWidth(width - gap * 2);
        imageRectangle.setHeight(height - gap * 2);
        imageRectangle.setFill(Color.PINK);
        imageRectangle.setArcWidth(12);
        imageRectangle.setArcHeight(12);
        imageRectangle.setLayoutY(gap);
        imageRectangle.setLayoutX(gap);

        imageView.setFitWidth(width - gap * 4);
        imageView.setFitHeight(width - gap * 4);
        imageView.setLayoutY((12 * height / 90) * (height - gap * 2) / (width - gap * 4));
        imageView.setLayoutX((8 * width / 50) * (width - gap * 2) / (width - gap * 4));
    }

    private void initLabel() {
        label.setFont(Font.font("Segoe UI", FontWeight.BOLD, FontPosture.REGULAR, fontSize));
        label.setTextFill(Color.WHITE);
    }

    private void calculateTextRectangle() {
        this.mainRectangle.setWidth(width + gap * 4 + this.label.getText().length() * fontSize / 2);

        this.label.setLayoutX(width + gap);
        this.label.setLayoutY((height / 2) - fontSize / 2);
    }

    private void initTooltip() {
        tooltip.setFont(Font.font("Segoe UI", FontWeight.BOLD, FontPosture.REGULAR, tooltipFontSize));
    }

    public void setText(String text) {
        if (text == null) {
            if (label != null) {
                getChildren().remove(label);
                label = null;
            }
            return;
        }

        if (label == null) {
            label = new Label();
            getChildren().add(label);
            initLabel();
        }

        label.setText(text);
        calculateTextRectangle();

    }

    public void setTooltip(String text) {
        if (text == null) {
            if (tooltip != null) {
                Tooltip.uninstall(this, tooltip);
                tooltip = null;
            }
            return;
        }

        if (tooltip == null) {
            tooltip = new Tooltip(text);
            Tooltip.install(this, tooltip);
            initTooltip();
        }

        tooltip.setText(text);
    }

    public void toUpperCase() {
        this.tooltip.setText(this.tooltip.getText().toUpperCase());
        this.label.setText(this.label.getText().toUpperCase());
    }

    private Point2D locationTooltip() {

        Point2D location = super.localToScreen(super.getBoundsInLocal().getMinX(),
                super.getBoundsInLocal().getMinY());
        double x = location.getX() + super.getBoundsInLocal().getWidth() / 2 - tooltip.getWidth() / 2;
        double y = location.getY() + -tooltip.getHeight() * 0.7;
        return new Point2D(x, y);
    }

    public void setFill(Paint paint) {
        this.mainRectangle.setFill(paint);
    }

    public void setInnerFill(Paint paint) {
        this.imageRectangle.setFill(paint);
    }

    public void setImage(Image image) {
        imageView.setImage(image);
    }

    public void setFontSize(double size) {
        this.fontSize = size;
    }

    public void setTooltipFontSize(double size) {
        this.tooltipFontSize = size;
    }

    public ScaleTransition scale(double size) {
        Interpolator interpolator = Interpolator.SPLINE(0, 0.6, 0.3, 1);

        ScaleTransition scaleTransition = new ScaleTransition(Duration.seconds(0.1), this);
        scaleTransition.setToX(size);
        scaleTransition.setToY(size);
        scaleTransition.setInterpolator(interpolator);
        scaleTransition.play();
        return scaleTransition;
    }

    public FadeTransition fade(double value) {
        Interpolator interpolator = Interpolator.SPLINE(0, 0.6, 0.3, 1);

        FadeTransition fadeTransition = new FadeTransition(Duration.seconds(0.1), this);
        fadeTransition.setToValue(value);
        fadeTransition.setInterpolator(interpolator);
        fadeTransition.play();
        return fadeTransition;
    }

    public void setOnMouseEnter(EventHandler<MouseEvent> onMouseEntered) {
        this.onMouseEntered = onMouseEntered;
    }

    public void setOnMouseExit(EventHandler<MouseEvent> onMouseExited) {
        this.onMouseExited = onMouseExited;
    }

    public void setScaleOnHover(boolean scaleOnHover) {
        this.scaleOnHover = scaleOnHover;
    }

    public void setImageScaleX(double scaleX) {
        this.imageView.setScaleX(scaleX);
    }

    public void setImageScaleY(double scaleY) {
        this.imageView.setScaleY(scaleY);
    }

    public void setOnDisabled(EventHandler<Event> onDisable) {
        this.onDisabled = onDisable;
    }

    public void setOnEnabled(EventHandler<Event> onEnable) {
        this.onEnabled = onEnable;
    }

    public boolean getDisabled() {
        return this.buttonDisabled;
    }

    public void setButtonDisabled(boolean value) {
        this.buttonDisabled = value;

        if (buttonDisabled)
            onDisabled.handle(new Event(null));
        else
            onEnabled.handle(new Event(null));
    }

    public void translateImageX(double x) {
        this.imageView.setTranslateX(x);
    }
}
