package com.c1t45.view;

import com.c1t45.view.Generators.NameGenerator;

import javafx.scene.paint.Color;

public class EnemyPlayer extends Player {

    public EnemyPlayer(byte id, String name, Color color) {
        super(id, name, color);
    }

    public static void fillPlayers(Player[] players, Color[] colors, byte playerCount) {
        byte offset;
        NameGenerator generator = new NameGenerator();

        for (offset = 1; offset < playerCount; offset++) {
            EnemyPlayer enemy = new EnemyPlayer((byte) offset, generator.generate(), colors[offset]);
            players[offset] = enemy;
        }
    }

}
