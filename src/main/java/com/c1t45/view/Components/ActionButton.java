package com.c1t45.view.Components;

import com.c1t45.view.Packages.ImagePackage;
import com.c1t45.view.Utils.ImageUtils;

import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Paint;

public class ActionButton extends ImageButton {
    private byte count;
    private EventHandler<MouseEvent> onAction;

    public ActionButton(ImagePackage pack, byte count) {
        this(pack, 50, 90, 5);
        setText(Byte.toString(count));
        setInnerFill(Paint.valueOf("#101010"));
        this.count = count;
    }

    public ActionButton(ImagePackage pack) {
        this(pack, 50, 90, 5);
        setFontSize(0);
        setText(" ");
        this.count = -1;
        translateImageX(10);
    }

    private ActionButton(ImagePackage pack, double width, double height, double gap) {
        super(width * 0.8, height * 0.8, gap * 0.8);
        setTooltip(pack.name.toUpperCase());
        setFill(Paint.valueOf("#101010"));
        setInnerFill(Paint.valueOf("#101010"));
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
        this.onAction = null;

        ActionButton self = this;
        setOnMouseClicked((event) -> {
            if (!self.getDisabled() && count != 0 && onAction != null)
                onAction.handle(event);
        });
    }

    public void setOnAction(EventHandler<MouseEvent> handler) {
        this.onAction = handler;
    }

    public void setCount(byte _count) {
        if (_count != this.count) {
            super.scale(1.3, 0.3).setOnFinished((a) -> {
                super.scale(1, 1);
            });

        }
        setText(Byte.toString((this.count = _count)));
    }

    @Override
    public void setButtonDisabled(boolean value) {
        super.setButtonDisabled(value || count == 0);
    }

    public void setPackage(ImagePackage pack) {
        setTooltip(pack.name.toUpperCase());
        setImage(ImageUtils.colorChange(pack.image));
    }

}