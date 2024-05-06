package com.c1t45.view.Inventory;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Components.MaterialGroup;

import javafx.scene.layout.FlowPane;

public class InventoryPane {

    FlowPane flow;
    MaterialGroup[] materialGroups;
    LocalPlayer local;

    public InventoryPane(FlowPane inventoryFlow, LocalPlayer localPlayer) {
        flow = inventoryFlow;
        local = localPlayer;
        materialGroups = new MaterialGroup[5];

        for (byte index = (byte) 0; index < (byte) 5; index++) {
            materialGroups[index] = new MaterialGroup(Constants.Packages.materials[index]);
            flow.getChildren().add(materialGroups[index]);
        }

        local.addOnInventoryChange(() -> {
            byte[] mats = local.getMaterials();
            for (byte index = (byte) 0; index < mats.length; index++) {
                materialGroups[index].setCount(mats[index]);
            }
        });

    }
}
