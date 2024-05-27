package com.c1t45.view.Components.Buttons;

import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Packages.ImagePackage;
import com.c1t45.view.Utils.ImageUtils;

import javafx.scene.Cursor;
import javafx.scene.input.MouseButton;
import javafx.scene.paint.Color;

public class CountButton extends ActionButton {
    private Action<Byte> onAction;

    public CountButton(ImagePackage pack, double colorOpacity) {
        super(pack, (byte) 0);

        Color opacityColor = ImageUtils.colorOpacity((Color) pack.color, colorOpacity);

        setTooltip(pack.name.toUpperCase());
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


        CountButton self = this;
        setOnMouseClicked((event) -> {
            if (self.getDisabled())
                return;
            if (event.getButton() == MouseButton.PRIMARY) {
                setCount((byte) (super.count + 1), false);

            } else {
                setCount((byte) (super.count - 1), false);
            }
            if (onAction != null)
                onAction.action(super.count);

            setTooltip(null);

        });

        setTooltip(null);

    }

    public void setOnCountAction(Action<Byte> handler) {
        this.onAction = handler;
    }
}
