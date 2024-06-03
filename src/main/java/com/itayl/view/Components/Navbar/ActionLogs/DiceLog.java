package com.itayl.view.Components.Navbar.ActionLogs;

import com.itayl.view.Player;

public class DiceLog extends LogGroup {

    public DiceLog(Player player, int dice1, int dice2) {
        super(player, "rolled the dice [" + dice1 + ", " + dice2 + "]");
    }

}
