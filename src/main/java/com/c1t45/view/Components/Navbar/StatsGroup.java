package com.c1t45.view.Components.Navbar;

import com.c1t45.view.Components.Buttons.ImageButton;
import com.c1t45.view.Packages.ImagePackage;

import javafx.scene.paint.Paint;

class StatsGroup extends ImageButton {
    private byte count;

    public StatsGroup(ImagePackage landUI) {
        this(landUI, (byte) 1);
    }

    public StatsGroup(ImagePackage landUI, byte count) {
        super(30 * 1.3, 45 * 1.3, 3 * 1.3);

        setFontSize(20);
        setTooltipFontSize(10);
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