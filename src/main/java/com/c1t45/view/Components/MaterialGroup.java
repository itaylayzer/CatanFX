package com.c1t45.view.Components;

import com.c1t45.view.Packages.ImagePackage;

import javafx.scene.paint.Paint;

public class MaterialGroup extends ImageButton {
    private byte count;

    public MaterialGroup(ImagePackage pack) {
        super(50, 90, 5);

        setTooltip(pack.name.toUpperCase());
        setFill(Paint.valueOf("#101010"));
        setInnerFill(pack.color);
        setImage(pack.image);
        setCount(this.count);
    }

    public void setCount(byte count) {
        this.count = count;
        super.setText(Byte.toString(count));
    }
}
