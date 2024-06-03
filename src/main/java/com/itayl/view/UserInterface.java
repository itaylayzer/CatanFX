package com.itayl.view;

import com.itayl.view.Components.CatanBoard.DicePane;
import com.itayl.view.Components.Inventory.ButtonsPane;
import com.itayl.view.Components.Inventory.InventoryPane;

import javafx.scene.effect.BoxBlur;
import javafx.scene.effect.Effect;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

public class UserInterface {

    private InventoryPane inventoryPane;
    private ButtonsPane buttonsPane;
    private HBox inventoryBox;

    public UserInterface(LocalPlayer localPlayer,
            FlowPane materialsBox,
            FlowPane buttonsFlow,
            HBox inventoryBox,
            DicePane dices,
            VBox actionsBox) {

        inventoryPane = new InventoryPane(materialsBox, localPlayer);

        buttonsPane = new ButtonsPane(buttonsFlow, localPlayer);
        buttonsPane.setOnDice((setDisable) -> {
            setDisable.action(true);
            dices.randRoll((c1, c2) -> {

                setDisable.action(false);
            });
        });

        this.inventoryBox = inventoryBox;
    }

    public InventoryPane getInventory() {
        return this.inventoryPane;
    }

    public void setDisabled(boolean isDisabled) {
        this.inventoryBox.setMouseTransparent(isDisabled);
        if (isDisabled) {
            Effect effect = new BoxBlur(this.inventoryBox.getWidth(), this.inventoryBox.getHeight(), 10);
            this.inventoryBox.setEffect(effect);
        } else {
            this.inventoryBox.setEffect(null);

        }
        this.inventoryBox.setOpacity(isDisabled ? 0.5 : 1);
    }

}
