package com.c1t45.view;

import java.util.ArrayList;
import java.util.List;

import com.c1t45.controller.SocketClient;
import com.c1t45.view.Constants.Actions.TransferMaterials;
import com.c1t45.view.CatanBoard.CatanBoard;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Navbar.BankPane;

import javafx.scene.paint.Color;

public class LocalPlayer extends Player {

    public static LocalPlayer local;

    private byte actionable;
    private List<Action<Byte>> onActionableChange;
    private SocketClient client;

    private boolean initMode;
    private BankPane bank;

    public LocalPlayer(byte id, String localeName, Color color, SocketClient client) {
        super(id, localeName, color);

        local = this;

        this.client = client;
        this.actionable = 0;
        this.initMode = true;
        this.onActionableChange = new ArrayList<>();

        this.update();

    }

    public void initialize() {
        init_pickVertexAndEdge(TransferMaterials.NONE, () -> {
            this.endTurn();
        });
    }

    private void init_pickVertexAndEdge(TransferMaterials transferMaterials, Runnable finished) {
        CatanBoard board = CatanBoard.getInstance();
        board.cancelCurrentPick(false);
        board.pickVertex((value) -> {
            return Player.houseDontBelong(value);
        }, () -> {

        }, (picked_house) -> {
            this.buyHouse(transferMaterials, picked_house);
            CatanBoard.addHouse(picked_house, getColor());

            board.pickEdge((value) -> {
                return Player.roadDontBelong(value);
            }, () -> {
            }, (picked_road) -> {
                byte[] fromto = board.seperateEdge(picked_road);
                this.buyRoad(false, fromto[0], fromto[1]);
                CatanBoard.addRoad(picked_road, getColor());
                finished.run();
            });
        });
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
            setActionable(client.getActionable());
            setMaterials(client.getMaterials(this.id));
            setDevelopements(client.getDevelopments((byte) 5));

            if (bank == null)
                return;
            bank.setMaterials(client.getMaterials((byte) 5));
            bank.setDevelopements(client.getDevelopments((byte) 5));
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
    }

    public void buyHouse(TransferMaterials transferMaterials, byte vertex) {

        try {
            client.storeHouse(transferMaterials, vertex);
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

    public void buyRoad(boolean transferMaterials, byte from, byte to) {
        try {
            client.storeRoad(transferMaterials, from, to);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void endTurn() {
        Player.moveTurn();
        setActionable(actionable);
        try {
            client.endTurn((t) -> {
                return !myTurn();
            }, (t) -> {
                setActionable(actionable);

                if (this.initMode) {
                    this.initMode = false;
                    init_pickVertexAndEdge(TransferMaterials.TO, () -> {
                        this.endTurn();
                    });
                } else {
                    update();
                }
            });

        } catch (Exception ex) {
            System.err.println(ex);
        }

    }

    public boolean turnable() {
        return super.myTurn() && !this.initMode;
    }

    public static void clear() {
        local = null;
    }

    public static void setBank(BankPane bank) {
        local.bank = bank;
    }
}