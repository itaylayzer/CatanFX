package com.c1t45.view.Components;

import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Packages.ImagePackage;
import javafx.scene.paint.Paint;

public class DevelopmentGroup extends ActionButton {

    public DevelopmentGroup(ImagePackage pack) {
        super(new ImagePackage(pack.name, Paint.valueOf("#101010"), pack.image), (byte) 0);
        super.setButtonDisabled(true);
    }

    public DevelopmentGroup(ImagePackage pack, Action<DevelopmentGroup> event) {
        super(new ImagePackage(pack.name, Paint.valueOf("#101010"), pack.image), (byte) 0);
        super.setButtonDisabled(false);
        super.setOnAction((eve) -> {
            event.action(this);
        });
    }
}
