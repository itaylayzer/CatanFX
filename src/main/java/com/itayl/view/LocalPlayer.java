package com.itayl.view;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import com.itayl.controller.Constants.ServerCodes;
import com.itayl.controller.SocketClient;
import com.itayl.view.Components.CatanBoard.CatanBoard;
import com.itayl.view.Components.Navbar.BankPane;
import com.itayl.view.Constants.Actions.TransferMaterials;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Packages.LinePackage;
import com.itayl.view.Packages.VertexPackage;

import javafx.application.Platform;
import javafx.scene.paint.Color;

public class LocalPlayer extends Player {

    public static LocalPlayer local;

    private byte actionable;
    private List<Action<Byte>> onActionableChange;
    private List<Action<Byte[]>> onAmountsChange;
    private SocketClient client;
    private List<Byte> roads;

    private char initMode;
    private BankPane bank;

    private byte[] amounts;

    public LocalPlayer(byte id, String localeName, Color color, SocketClient client) {
        super(id, localeName, color);

        local = this;

        this.client = client;
        this.actionable = 0;
        this.initMode = 2;
        this.onActionableChange = new ArrayList<>();
        onAmountsChange = new ArrayList<>();
        amounts = new byte[3];
        this.roads = new ArrayList<>();

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
            return Player.houseDontBelong(value) && validHousesToBuy(value, false);
        }, () -> {

        }, (picked_house) -> {
            this.buyHouse(transferMaterials, picked_house);
            CatanBoard.addHouse(picked_house, getColor());

            board.pickEdge((value) -> {
                VertexPackage vertex = board.getVertex(picked_house);
                boolean found = false;

                for (var line : vertex.lines) {
                    found = found || line.offset == value;
                }

                return Player.roadDontBelong(value) && found;
            }, () -> {
            }, (picked_road) -> {
                byte[] fromto = board.seperateEdge(picked_road);
                this.buyRoad(false, picked_road, fromto[0], fromto[1]);
                CatanBoard.addRoad(picked_road, getColor());
                finished.run();
            }, false, false);
        }, false, false);
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

    public void addOnAmountsChange(Action<Byte[]> eve) {
        this.onAmountsChange.add(eve);
    }

    public void addOnActionableEvent(Action<Byte> eve, boolean call) {
        onActionableChange.add(eve);
        if (call)
            eve.action(actionable);
    }

    private void setAmounts(byte[] _amounts) {
        this.amounts = _amounts;
        for (var event : onAmountsChange) {
            event.action(new Byte[] { this.amounts[0], this.amounts[1], this.amounts[2] });
        }
    }

    public void update() {
        try {
            setActionable(client.getActionable());
            setMaterials(client.getMaterials(this.id));
            setDevelopements(client.getDevelopments(this.id));
            setAmounts(client.getAmounts());
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
            super.buyHouse(vertex);
            client.storeHouse(transferMaterials, vertex);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void buyCity(byte vertex) {
        try {
            super.buyCity(vertex);
            client.storeCity(vertex);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();
    }

    public void buyRoad(boolean transferMaterials, byte offset, byte from, byte to) {
        try {
            super.buyRoad(offset);
            setAchivementIDS((byte) 1, client.storeRoad(transferMaterials, from, to));
            roads.add(offset);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public Set<Byte> validRoadsToBuy() {
        Set<Byte> set = new TreeSet<Byte>();
        CatanBoard instance = CatanBoard.getInstance();

        for (var edge : this.roads) {
            LinePackage line = instance.getLine(edge);

            for (var xline : instance.getVertex(line.from).lines) {
                set.add(xline.offset);
            }

            for (var xline : instance.getVertex(line.to).lines) {
                set.add(xline.offset);
            }

        }

        set.removeAll(this.roads);
        return set;
    }

    public boolean validHousesToBuy(byte offset) {
        return validHousesToBuy(offset, true);
    }

    private boolean validHousesToBuy(byte offset, boolean hasRoad) {
        boolean dontHaveHouseNear = true, hasLineBelongs = !hasRoad;
        CatanBoard instance = CatanBoard.getInstance();

        for (var line : instance.getVertex(offset).lines) {
            byte other = (byte) (line.from + line.to - offset);
            if (!Player.houseDontBelong((byte) other)) {
                dontHaveHouseNear = false;
            }
            if (Player.hasRoad(id, line.offset)) {
                hasLineBelongs = true;
            }

        }
        return dontHaveHouseNear && hasLineBelongs;
    }

    public void endTurn() {
        Player.moveTurn();
        setActionable(actionable);
        try {
            client.endTurn((t) -> {
                return !myTurn();
            }, (bytes) -> {
                handleBot(bytes);
            }, (t) -> {
                setActionable(actionable);

                if (this.initMode > 1) {
                    this.initMode = 1;
                    init_pickVertexAndEdge(TransferMaterials.TO, () -> {
                        this.endTurn();
                    });
                } else {
                    this.initMode = 0;
                    update();
                }
            });

        } catch (Exception ex) {
            System.err.println(ex);
        }

    }

    private void handleBot(byte[] response) {
        System.out.println("Player.getTurnID() = " + Player.getTurnID());
        EnemyPlayer enemy = EnemyPlayer.enemies[Player.getTurnID() - 1];
        switch (response[0]) {
            case ServerCodes.TURN:
                Player.setTurnID(response[1]);
                break;
            case ServerCodes.UPDATE_SETTLEMENT: {
                byte house = (byte) (response[1] - Constants.AREAS);
                enemy.buyHouse(house);
                Platform.runLater(() -> {
                    CatanBoard.addHouse(house,
                            enemy.getColor());
                });
                break;
            }
            case ServerCodes.UPDATE_ROAD: {
                byte road = CatanBoard.getInstance().joinEdge((byte) (response[1] - Constants.AREAS),
                        (byte) (response[2] - Constants.AREAS));
                enemy.buyRoad(road);

                Platform.runLater(() -> {
                    CatanBoard.addRoad(road,
                            enemy.getColor());
                });
                break;
            }
            case ServerCodes.DICE:
                Platform.runLater(() -> {
                    CatanBoard.dicePane.setRoll(response[1] & 0x0F, response[1] >> 4);
                });
                this.update();
                EnemyPlayer.update(this.client);
                break;

        }
    }

    public boolean turnable() {
        return super.myTurn() && this.initMode == 0;
    }

    public static void clear() {
        local = null;
    }

    public static void setBank(BankPane bank) {
        local.bank = bank;
    }

    public void moveRobber(byte robber, byte playerIndex) {
        try {
            setAchivementIDS((byte) 0, client.moveRobber(robber, playerIndex));
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
    }

    public byte[] getDevelopments() {
        return this.devcards;
    }

    public void usedDevCard(byte offset) {
        try {
            System.out.println(Arrays.toString(client.usedDevCard(offset)));
        } catch (Exception ex) {

        }
        update();
    }

    public void buyDevCard() {
        try {

            client.storeDevCard();
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();

    }

    public void yearOfPlant(Byte firstMat, Byte secondMat) {
        try {
            client.yearOfPlant(firstMat, secondMat);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();
    }

    public void monopol(Byte matIndex) {
        try {
            client.monopol(matIndex);
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
        update();
    }

}