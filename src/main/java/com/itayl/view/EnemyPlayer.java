package com.itayl.view;

import com.itayl.controller.SocketClient;
import com.itayl.view.Generators.NameGenerator;

import javafx.scene.paint.Color;

public class EnemyPlayer extends Player {
    public static EnemyPlayer[] enemies;
    static {
        System.out.println("created ");
        enemies = new EnemyPlayer[3];
    }

    public EnemyPlayer(byte id, String name, Color color) {
        super(id, name, color);

        System.out.println("yes yes " + id);
        EnemyPlayer.enemies[id - 1] = this;
    }

    public static void fillPlayers(Player[] players, Color[] colors, byte playerCount) {
        byte offset;
        NameGenerator generator = new NameGenerator();

        for (offset = 1; offset < playerCount; offset++) {
            EnemyPlayer enemy = new EnemyPlayer((byte) offset, generator.generate(), colors[offset]);
            players[offset] = enemy;
        }
    }

    public static void update(SocketClient client) {
        for (EnemyPlayer enemy : enemies) {
            if (enemy == null)
                continue;

            enemy.updateFromSocket(client);
        }
    }

    private void updateFromSocket(SocketClient client) {
        try {
            this.setMaterials(client.getMaterials(id));
            this.setDevelopements(client.getDevelopments(id));
        } catch (Exception exp) {
            exp.printStackTrace(System.err);
        }
    }

}
