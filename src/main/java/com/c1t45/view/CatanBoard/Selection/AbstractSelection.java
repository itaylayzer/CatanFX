package com.c1t45.view.CatanBoard.Selection;

import com.c1t45.view.Interfaces.Action;

import javafx.scene.layout.VBox;

public abstract class AbstractSelection {
    public abstract void selection(VBox box, Action<Byte> onFinish, boolean cancellable);
}
