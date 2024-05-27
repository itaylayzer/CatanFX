package com.c1t45.view.Components.Navbar;

import com.c1t45.view.Components.Buttons.ImageButton;
import com.c1t45.view.Packages.ImagePackage;

import javafx.scene.paint.Paint;

class MiniMaterialGroup extends ImageButton {
    private byte count;
    private static final double multiplyFactor = 1;

    public MiniMaterialGroup(ImagePackage landUI) {
        this(landUI, (byte) 1);
    }

    public MiniMaterialGroup(ImagePackage landUI, byte count) {
        super(30 * multiplyFactor, 45 * multiplyFactor, 3 * multiplyFactor);

        setFontSize(20);
        setTooltipFontSize(10 * multiplyFactor);
        setTooltip(landUI.name.toUpperCase());
        setFill(Paint.valueOf("#101010"));
        setInnerFill(landUI.color);
        setImage(landUI.image);
        setCount(this.count);

    }

    public void setCount(byte count) {
        this.count = count;
        super.setText(Byte.toString(count));
    }
}