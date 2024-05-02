package com.c1t45.view;

import com.c1t45.view.CatanBoard.DicePane;
import com.c1t45.view.Inventory.ButtonsPane;
import com.c1t45.view.Inventory.InventoryPane;

import javafx.scene.layout.FlowPane;
import javafx.scene.layout.VBox;

public class UserInterface {

    private LocalPlayer player;
    private InventoryPane inventoryPane;
    private ButtonsPane buttonsPane;

    public UserInterface(LocalPlayer localPlayer,
            FlowPane inventoryFlow,
            FlowPane buttonsFlow,
            DicePane dices,
            VBox actionsBox) {

        inventoryPane = new InventoryPane(inventoryFlow);

        buttonsPane = new ButtonsPane(buttonsFlow, (player = localPlayer));
        buttonsPane.setOnDice((setDisable) -> {
            setDisable.action(true);
            dices.randRoll((c1, c2) -> {

                setDisable.action(false);
            });
        });
    }

    public void Render() {
        inventoryPane.Render(player.getMaterials());
    }

    public InventoryPane getInventory() {
        return this.inventoryPane;
    }

    // LEFTON:
    // private void addActionLog(LogGroup logGroup, Image image) {
    // logGroup.setImage(ImageUtils.colorChange(image), 14.0, 14.0);
    // actionsBox.getChildren().add(logGroup);
    // }
}
