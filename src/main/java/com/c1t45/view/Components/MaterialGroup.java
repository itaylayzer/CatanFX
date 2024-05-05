package com.c1t45.view.Components;

import com.c1t45.view.Packages.ImagePackage;

import javafx.application.Platform;
import javafx.scene.paint.Paint;

public class MaterialGroup extends ImageButton {

    private byte count;

    public MaterialGroup(ImagePackage pack) {
        super(50, 90, 5);

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
        this.count = _count;
        super.setText(Byte.toString(_count));
    }
}
