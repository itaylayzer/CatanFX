package com.itayl.view.Components.Inventory;

import com.itayl.view.Constants;
import com.itayl.view.LocalPlayer;
import com.itayl.view.Player;
import com.itayl.view.Components.Buttons.ActionButton;
import com.itayl.view.Components.CatanBoard.CatanBoard;
import com.itayl.view.Constants.Actions.TransferMaterials;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Utils.BytesUtils;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.layout.FlowPane;

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

        rollAction = new ActionButton(Constants.Packages.rolldice);
        roadAction = new ActionButton(Constants.Packages.roads, (byte) 15);
        houseAction = new ActionButton(Constants.Packages.house, (byte) 5);
        cityAction = new ActionButton(Constants.Packages.city, (byte) 4);
        devcardAction = new ActionButton(Constants.Packages.devcard);
        tradeAction = new ActionButton(Constants.Packages.trade);

        ObjectProperty<Boolean> doRolls = new SimpleObjectProperty<Boolean>(true);

        buttonsFlow.getChildren().add(tradeAction);
        buttonsFlow.getChildren().add(devcardAction);
        buttonsFlow.getChildren().add(cityAction);
        buttonsFlow.getChildren().add(houseAction);
        buttonsFlow.getChildren().add(roadAction);
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
                return Player.houseDontBelong(value) && player.validHousesToBuy(value);
            }, () -> {
                houseAction.setButtonDisabled(false);
            }, (picked) -> {
                houseAction.setButtonDisabled(false);
                System.out.println("picked-dot=" + picked);

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
                cityAction.setButtonDisabled(false);
            }, (picked) -> {
                cityAction.setButtonDisabled(false);
                System.out.println("picked-dot=" + picked);
                player.buyCity(picked);
                CatanBoard.addCity(picked, player.getColor());
            });
        });

        roadAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            roadAction.setButtonDisabled(true);
            board.cancelCurrentPick(false);

            board.pickEdge((value) -> {
                return Player.roadDontBelong(value) && player.validRoadsToBuy().contains(value);
            }, () -> {
                roadAction.setButtonDisabled(false);
            }, (picked) -> {
                roadAction.setButtonDisabled(false);
                System.out.println("picked-edge=" + picked);
                byte[] fromto = board.seperateEdge(picked);
                player.buyRoad(true, picked, fromto[0], fromto[1]);
                CatanBoard.addRoad(picked, player.getColor());
            });
        });
        tradeAction.setOnAction((event) -> {
            CatanBoard board = CatanBoard.getInstance();
            board.trade(player, (dealType) -> {
                byte from = (byte) (dealType & 0x07);
                byte to = (byte) (dealType >> 3);
                System.out.println("from=" + from + " to=" + to);
                player.deal(dealType);
            });
        });
        devcardAction.setOnAction((event) -> {
            player.buyDevCard();
        });

        player.addOnAmountsChange((amounts) -> {
            roadAction.setCount(amounts[0]);
            houseAction.setCount(amounts[1]);
            cityAction.setCount(amounts[2]);
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
