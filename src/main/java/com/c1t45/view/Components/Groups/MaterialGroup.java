package com.c1t45.view.Components.Groups;

import com.c1t45.view.Components.Buttons.ImageButton;
import com.c1t45.view.Packages.ImagePackage;

import javafx.scene.paint.Paint;

public class MaterialGroup extends ImageButton {

    private byte count;

    public MaterialGroup(ImagePackage pack) {
        super(50 * 0.8, 90 * 0.8, 5 * 0.8);

        setTooltip(pack.name.toUpperCase());
        setFill(Paint.valueOf("#101010"));
        setInnerFill(pack.color);
        setImage(pack.image);
        setCount((count = (byte) 0));
    }

    public void setCount(byte _count) {
        if (_count != this.count) {
            super.scale(1.3, 0.3).setOnFinished((a) -> {
                super.scale(1, 1);
            });

        }

        super.setText(Byte.toString((this.count = _count)));
    }
}
