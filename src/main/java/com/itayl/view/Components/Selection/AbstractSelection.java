package com.itayl.view.Components.Selection;

import com.itayl.view.Interfaces.Action;

import javafx.scene.layout.VBox;

public abstract class AbstractSelection {
    public abstract void selection(VBox box, Action<Byte> onFinish, boolean cancellable);
}
