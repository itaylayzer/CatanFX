package com.c1t45.view;

import com.c1t45.model.LocalePlayer;
import com.c1t45.model.Player;
import com.c1t45.view.CatanBoard.DicePane;
import com.c1t45.view.Inventory.ButtonsPane;
import com.c1t45.view.Inventory.InventoryPane;
import com.c1t45.view.Navbar.ActionLogs.CityLog;
import com.c1t45.view.Navbar.ActionLogs.DiceLog;
import com.c1t45.view.Navbar.ActionLogs.HouseLog;
import com.c1t45.view.Navbar.ActionLogs.LogGroup;
import com.c1t45.view.Navbar.ActionLogs.RoadLog;
import com.c1t45.view.Utils.ImageUtils;

import javafx.scene.image.Image;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.VBox;

public class UserInterface {

    private LocalePlayer player;
    private InventoryPane inventoryPane;
    private ButtonsPane buttonsPane;
    private VBox actionsBox;

    public UserInterface(LocalePlayer localPlayer,
            FlowPane inventoryFlow,
            FlowPane buttonsFlow,
            DicePane dices,
            VBox actionsBox) {

        player = localPlayer;
        this.actionsBox = actionsBox;

        inventoryPane = new InventoryPane(inventoryFlow);

        buttonsPane = new ButtonsPane(buttonsFlow, player);
        buttonsPane.setOnDice((setDisable) -> {
            setDisable.action(true);
            Render();
            dices.roll((c1, c2) -> {

                setDisable.action(false);
                player.rollAction(c1, c2);
            });
        });

        Player.addOnRoll((c1, c2) -> {
            addActionLog(new DiceLog(player, (int) c1, c2), Constants.Images.rolldice);
        });
        Player.addOnCityChange((city, player) -> {
            addActionLog(new CityLog(player), Constants.Images.city);
        });
        Player.addOnHouseChange((house, player) -> {
            addActionLog(new HouseLog(player), Constants.Images.house);
        });
        Player.addOnRoadChange((house, player) -> {
            addActionLog(new RoadLog(player), Constants.Images.roads);
        });

        player.addOnInventoryChange(() -> {
            Render();
        });
        Render();
    }

    public void Render() {
        inventoryPane.Render(player.getMaterials());
        buttonsPane.Render();
    }

    public InventoryPane getInventory() {
        return this.inventoryPane;
    }

    private void addActionLog(LogGroup logGroup, Image image) {
        logGroup.setImage(ImageUtils.colorChange(image), 14.0, 14.0);
        actionsBox.getChildren().add(logGroup);
    }
}
