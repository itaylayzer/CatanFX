package com.c1t45.view;

import com.c1t45.view.CatanBoard.DicePane;
import com.c1t45.view.Inventory.ButtonsPane;
import com.c1t45.view.Inventory.InventoryPane;

import javafx.scene.layout.FlowPane;
import javafx.scene.layout.VBox;

public class UserInterface {

    private InventoryPane inventoryPane;
    private ButtonsPane buttonsPane;

    public UserInterface(LocalPlayer localPlayer,
            FlowPane inventoryFlow,
            FlowPane buttonsFlow,
            DicePane dices,
            VBox actionsBox) {

        inventoryPane = new InventoryPane(inventoryFlow, localPlayer);

        buttonsPane = new ButtonsPane(buttonsFlow, localPlayer);
        buttonsPane.setOnDice((setDisable) -> {
            setDisable.action(true);
            dices.randRoll((c1, c2) -> {

                setDisable.action(false);
            });
        });
    }

    public InventoryPane getInventory() {
        return this.inventoryPane;
    }

}
