package com.c1t45.view.Navbar.ActionLogs;

import com.c1t45.model.Player;

public class DiceLog extends LogGroup {

    public DiceLog(Player player, int dice1, int dice2) {
        super(player, "rolled the dice [" + dice1 + ", " + dice2 + "]");
    }

}
