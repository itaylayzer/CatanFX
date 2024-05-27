package com.c1t45.view.Components.Buttons;

import com.c1t45.view.Packages.ImagePackage;
import com.c1t45.view.Utils.ImageUtils;

import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;

public class SelectButton extends ImageButton {

    private EventHandler<MouseEvent> onAction;

    public SelectButton(ImagePackage pack) {
        this(pack, 50, 90, 5, 1);
    }

    public SelectButton(ImagePackage pack, double opacity) {
        this(pack, 50, 90, 5, opacity);
    }

    public SelectButton(ImagePackage pack, boolean dontColor) {
        this(pack, 50, 90, 5, 1);
        if (dontColor) {
            setFill(Paint.valueOf("#101010"));
            setInnerFill(Paint.valueOf("#101010"));
        }
    }

    private SelectButton(ImagePackage pack, double width, double height, double gap, double colorOpacity) {
        super(width * 0.8, height * 0.8, gap * 0.8);

        Color opacityColor = ImageUtils.colorOpacity((Color) pack.color, colorOpacity);

        setTooltip(pack.name.toUpperCase());
        setFill(opacityColor);
        setInnerFill(opacityColor);
        setImage(ImageUtils.colorChange(pack.image));
        super.setCursor(Cursor.HAND);
        setOnDisabled((event) -> {
            super.setCursor(Cursor.DEFAULT);
            this.scale(0.9);
            this.fade(0.6);
            ;
            setScaleOnHover(false);
        });
        setOnEnabled((event) -> {
            super.setCursor(Cursor.HAND);
            this.fade(1);
            this.scale(1);
            setScaleOnHover(true);
        });
        setFontSize(0);
        translateImageX(10);

        this.onAction = null;
        setText("10");
        SelectButton self = this;
        setOnMouseClicked((event) -> {
            if (!self.getDisabled() && onAction != null)
                onAction.handle(event);
        });
    }

    public void setOnAction(EventHandler<MouseEvent> handler) {
        this.onAction = handler;
    }

}