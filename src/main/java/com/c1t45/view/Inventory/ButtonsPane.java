package com.c1t45.view.Inventory;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.scene.layout.FlowPane;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.CatanBoard.CatanBoard;
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

        player.addOnActionableEvent(actionable -> {
            houseAction.setButtonDisabled(!(player.myTurn() && BytesUtils.bit((byte) actionable, (byte) 0)));
            cityAction.setButtonDisabled(!(player.myTurn() && BytesUtils.bit((byte) actionable, (byte) 1)));
            roadAction.setButtonDisabled(!(player.myTurn() && BytesUtils.bit((byte) actionable, (byte) 2)));
            devcardAction.setButtonDisabled(!(player.myTurn() && BytesUtils.bit((byte) actionable, (byte) 3)));
            tradeAction.setButtonDisabled(!player.myTurn());
            rollAction.setButtonDisabled(!player.myTurn());
        });

    }

    public void setOnDice(Action<Action<Boolean>> onDice) {
        this.onDice = onDice;
    }

}
