package com.c1t45.view;

import java.util.ArrayList;
import java.util.List;

import com.c1t45.controller.SocketClient;
import com.c1t45.view.Interfaces.Action;

import javafx.scene.paint.Color;

public class LocalPlayer extends Player {

    private byte actionable;
    private List<Action<Byte>> onActionableChange;
    private SocketClient client;

    public LocalPlayer(byte id, String localeName, Color color, SocketClient client) {
        super(id, localeName, color);
        this.client = client;
        this.actionable = 0;
        onActionableChange = new ArrayList<>();

        this.update();
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
        this.addOnActionableEvent(eve, false);

    }

    public void addOnActionableEvent(Action<Byte> eve, boolean call) {
        onActionableChange.add(eve);
        if (call)
            eve.action(actionable);
    }

    private void update() {
        try {
            System.err.println("get Actionable!");
            setActionable(client.getActionable());
            // setMaterials(client.getMaterials(this.id));
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
    }

    public void buyHouse(byte vertex) {
        try {
            client.storeHouse(vertex);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void buyCity(byte vertex) {
        try {
            client.storeCity(vertex);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void buyRoad(byte from, byte to) {
        try {
            client.storeRoad(from, to);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void endTurn() {
        Player.moveTurn();
        setActionable(actionable);
        try {
            System.out.println("ending turn");
            client.endTurn((t) -> {
                return !this.myTurn();
            }, (t) -> {
                setActionable(actionable);
            });
            System.out.println("after ending turn");

        } catch (Exception ex) {
            System.err.println(ex);
        }

    }
}