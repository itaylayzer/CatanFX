package com.c1t45.view;

import java.util.ArrayList;
import java.util.List;

import com.c1t45.view.Interfaces.Action;
import javafx.scene.paint.Color;

public class LocalPlayer extends Player {

    private byte actionable;
    private List<Action<Byte>> onActionableChange;

    public LocalPlayer(byte id, String localeName, Color color) {
        super(id, localeName, color);
        this.actionable = 0;
        onActionableChange = new ArrayList<>();
    }

    public boolean hasHouse(Byte value) {
        return super.hasHouse(id, value);
    }

    public boolean hasCity(Byte value) {
        return super.hasCity(id, value);
    }

    public void setActionable(byte bites) {
        actionable = bites;

        for (var event : onActionableChange) {
            event.action(actionable);
        }
    }

    public void addOnActionableEvent(Action<Byte> eve) {
        onActionableChange.add(eve);
    }
}