package com.itayl.view.Components.Groups;

import com.itayl.view.Components.Buttons.ActionButton;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Packages.ImagePackage;

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
