package com.c1t45.view.Inventory;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.layout.FlowPane;

import com.c1t45.model.Player;
import com.c1t45.model.Interfaces.Action;
import com.c1t45.view.Constants;
import com.c1t45.view.CatanBoard.CatanBoard;

public class ButtonsPane {
    private Action<Action<Boolean>> onDice;
    private Player player;

    private ActionButton houseAction;
    private ActionButton cityAction;
    private ActionButton roadAction;
    private ActionButton devcardAction;
    private ActionButton tradeAction;
    private ActionButton rollAction;

    public ButtonsPane(FlowPane buttonsFlow, Player player) {
        this.player = player;
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
            if (doRolls.getValue()) {
                CatanBoard board = CatanBoard.getInstance();
                board.cancelCurrentPick(false);
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
                rollAction.setPackage(Constants.Packages.rolldice);
            }

        });

        houseAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            houseAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);

            board.pickVertex((value, index) -> {
                return Player.houseDontBelong(value);
            }, () -> {
                Render();

            }, (picked) -> {
                Render();
                System.out.println("picked-dot=" + picked);
                houseAction.decrease();
                player.buyHouse(picked);
            });
        });

        cityAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            cityAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);
            board.pickVertex((value, index) -> {
                return player.hasHouse(value) && !player.hasCity(value);
            }, () -> {
                Render();
            }, (picked) -> {
                Render();
                System.out.println("picked-dot=" + picked);
                cityAction.decrease();
                player.buyCity(picked);
            });
        });

        roadAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            roadAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);
            board.pickEdge((value, index) -> {
                return Player.roadDontBelong(value);
            }, () -> {
                Render();
            }, (picked) -> {
                Render();
                System.out.println("picked-line=" + picked);
                roadAction.decrease();
                player.buyRoad(picked);
            });
        });

        devcardAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            board.cancelCurrentPick(false);
            cityAction.setButtonDisabled(true);
            player.buyDevCard();
        });

    }

    public void setOnDice(Action<Action<Boolean>> onDice) {
        this.onDice = onDice;
    }

    public void Render() {
        cityAction.setButtonDisabled(!player.enoughMaterials(Constants.Store.city));
        houseAction.setButtonDisabled(!player.enoughMaterials(Constants.Store.house));
        roadAction.setButtonDisabled(!player.enoughMaterials(Constants.Store.road));
        devcardAction.setButtonDisabled(!player.enoughMaterials(Constants.Store.developmentCard));
    }

}
