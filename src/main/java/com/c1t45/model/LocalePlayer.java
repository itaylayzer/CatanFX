package com.c1t45.model;

import javafx.scene.paint.Color;

import java.util.Random;

import com.c1t45.controller.GameController;
import com.c1t45.view.Packages.NotificationPackage;

public class LocalePlayer extends Player {

    public LocalePlayer(byte id, String localeName, Color color) {
        super(id, localeName, color);

        this.materials[0] = 9;
        this.materials[1] = 9;
        this.materials[2] = 9;
        this.materials[3] = 9;
        this.materials[4] = 9;
    }

    public void random() {
        Random rand = new Random();

        this.materials[0] = (byte) rand.nextInt(0, 6);
        this.materials[1] = (byte) rand.nextInt(0, 6);
        this.materials[2] = (byte) rand.nextInt(0, 6);
        this.materials[3] = (byte) rand.nextInt(0, 6);
        this.materials[4] = (byte) rand.nextInt(0, 6);
        callOnInventoryChange();

    }

    public void clear() {
        this.materials[0] = 0;
        this.materials[1] = 0;
        this.materials[2] = 0;
        this.materials[3] = 0;
        this.materials[4] = 0;
        callOnInventoryChange();
    }

    @Override
    protected void addMaterials(byte[] addedMaterials) {
        super.addMaterials(addedMaterials);
        GameController.addNotification(new NotificationPackage("Added Materials", Color.WHITE));
    }
}
