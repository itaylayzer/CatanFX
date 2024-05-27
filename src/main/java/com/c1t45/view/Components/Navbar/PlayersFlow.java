package com.c1t45.view.Components.Navbar;

import com.c1t45.view.Player;
import com.c1t45.view.LocalPlayer;

import javafx.geometry.Insets;
import javafx.scene.layout.VBox;

public class PlayersFlow {

    private PlayerPane[] panes;
    private BankPane bank;

    public PlayersFlow(Player[] players, VBox playersVBox) {
        panes = new PlayerPane[players.length];
        bank = new BankPane();

        playersVBox.getChildren().add(bank);
        LocalPlayer.setBank(bank);

        for (byte index = (byte) (players.length - 1); index > -1; index--) {
            Player player = players[index];
            PlayerPane playerPane = new PlayerPane(player);
            playersVBox.getChildren().add(playerPane);
            panes[index] = playerPane;
        }

        Player.addOnAchievementsChange((bigestArmy, longestRoad) -> {
            for (byte index = 0; index < players.length; index++) {
                panes[index].setLongestRoad(longestRoad == index);
                panes[index].setBigestArmy(bigestArmy == index);
            }
        });

        playersVBox.setPadding(new Insets(10, 7, 10, 7));
        playersVBox.setSpacing(10);
    }

}
