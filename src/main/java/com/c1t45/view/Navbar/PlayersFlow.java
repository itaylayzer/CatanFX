package com.c1t45.view.Navbar;

import com.c1t45.model.Player;
import com.c1t45.view.Constants.Materials;

import javafx.geometry.Insets;
import javafx.scene.layout.VBox;

public class PlayersFlow {

    private PlayerPane[] panes;
    private BankPane bank;

    public PlayersFlow(Player[] players, VBox playersVBox) {
        panes = new PlayerPane[players.length];
        bank = new BankPane();

        playersVBox.getChildren().add(bank);

        // Rectangle rectangle = new Rectangle();
        // rectangle.setWidth(230);
        // rectangle.setHeight(2);
        // rectangle.setFill(Paint.valueOf("#656565"));
        // playersVBox.getChildren().add(rectangle);

        for (byte index = (byte) (players.length - 1); index > -1; index--) {
            Player player = players[index];
            PlayerPane playerPane = new PlayerPane(player);
            playersVBox.getChildren().add(playerPane);
            panes[index] = playerPane;
        }

        playersVBox.setPadding(new Insets(10, 7, 10, 7));
        playersVBox.setSpacing(10);
    }

    public void bankCount(Materials material, byte count) {
        this.bank.setCount(material, count);
    }

}
