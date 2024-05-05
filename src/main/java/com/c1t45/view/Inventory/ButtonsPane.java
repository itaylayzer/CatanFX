package com.c1t45.view.Inventory;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.layout.FlowPane;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Player;
import com.c1t45.view.CatanBoard.CatanBoard;
import com.c1t45.view.Constants.Actions.TransferMaterials;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Utils.BytesUtils;

public class ButtonsPane {
    private Action<Action<Boolean>> onDice;

    private ActionButton houseAction;
    private ActionButton cityAction;
    private ActionButton roadAction;
    private ActionButton devcardAction;
    private ActionButton tradeAction;
    private ActionButton rollAction;

    public ButtonsPane(FlowPane buttonsFlow, LocalPlayer player) {
        this.onDice = null;

        houseAction = new ActionButton(Constants.Packages.house, (byte) 5);
        cityAction = new ActionButton(Constants.Packages.city, (byte) 4);
        roadAction = new ActionButton(Constants.Packages.roads, (byte) 15);
        devcardAction = new ActionButton(Constants.Packages.devcards);
        tradeAction = new ActionButton(Constants.Packages.trade);
        rollAction = new ActionButton(Constants.Packages.rolldice);

        ObjectProperty<Boolean> doRolls = new SimpleObjectProperty<Boolean>(true);

        buttonsFlow.getChildren().add(tradeAction);
        buttonsFlow.getChildren().add(devcardAction);
        buttonsFlow.getChildren().add(roadAction);
        buttonsFlow.getChildren().add(houseAction);
        buttonsFlow.getChildren().add(cityAction);
        buttonsFlow.getChildren().add(rollAction);

        rollAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            board.cancelCurrentPick(false);

            if (doRolls.getValue()) {

                onDice.action(new Action<Boolean>() {
                    public void action(Boolean param) {
                        rollAction.setButtonDisabled(param);
                        if (!param) {
                            doRolls.setValue(false);
                            rollAction.setPackage(Constants.Packages.endturn);
                        }
                    }
                });
            } else {
                doRolls.setValue(true);
                player.endTurn();
                rollAction.setPackage(Constants.Packages.rolldice);
            }

        });

        houseAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            houseAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);

            board.pickVertex((value) -> {
                return Player.houseDontBelong(value);
            }, () -> {
            }, (picked) -> {
                System.out.println("picked-dot=" + picked);
                houseAction.decrease();

                player.buyHouse(TransferMaterials.FROM, picked);
                CatanBoard.addHouse(picked, player.getColor());
            });
        });

        cityAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            cityAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);
            board.pickVertex((value) -> {
                return player.hasHouse(value) && !player.hasCity(value);
            }, () -> {
            }, (picked) -> {
                System.out.println("picked-dot=" + picked);
                cityAction.decrease();
                player.buyCity(picked);
                CatanBoard.addCity(picked, player.getColor());
            });
        });

        roadAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            roadAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);

            board.pickEdge((value) -> {
                return Player.roadDontBelong(value) && player.roadable().contains(value);
            }, () -> {
            }, (picked) -> {
                System.out.println("picked-dot=" + picked);
                cityAction.decrease();
                byte[] fromto = board.seperateEdge(picked);
                player.buyRoad(true, picked, fromto[0], fromto[1]);
                CatanBoard.addRoad(picked, player.getColor());
            });
        });

        player.addOnActionableEvent(actionable -> {
            roadAction.setButtonDisabled(!(player.turnable() && BytesUtils.bit((byte) actionable, (byte) 0)));
            houseAction.setButtonDisabled(!(player.turnable() && BytesUtils.bit((byte) actionable, (byte) 1)));
            cityAction.setButtonDisabled(!(player.turnable() && BytesUtils.bit((byte) actionable, (byte) 2)));
            devcardAction.setButtonDisabled(!(player.turnable() && BytesUtils.bit((byte) actionable, (byte) 3)));
            tradeAction.setButtonDisabled(!player.turnable());
            rollAction.setButtonDisabled(!player.turnable());
        }, true);

    }

    public void setOnDice(Action<Action<Boolean>> onDice) {
        this.onDice = onDice;
    }

}
