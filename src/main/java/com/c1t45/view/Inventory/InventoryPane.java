package com.c1t45.view.Inventory;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Components.DevelopmentGroup;
import com.c1t45.view.Components.MaterialGroup;

import javafx.scene.layout.FlowPane;

public class InventoryPane {

    FlowPane flow;
    MaterialGroup[] materialGroups;
    DevelopmentGroup[] developmentsGroups;
    LocalPlayer local;

    public InventoryPane(FlowPane inventoryFlow, LocalPlayer localPlayer) {
        flow = inventoryFlow;
        local = localPlayer;
        materialGroups = new MaterialGroup[Constants.TOTAL_MATERIALS];
        developmentsGroups = new DevelopmentGroup[Constants.TOTAL_DEVELOPMENT_CARD - 1];

        for (byte index = (byte) 0; index < (byte) materialGroups.length; index++) {
            materialGroups[index] = new MaterialGroup(Constants.Packages.materials[index]);
            flow.getChildren().add(materialGroups[index]);
        }

        for (byte index = (byte) (0); index < developmentsGroups.length; index++) {

            developmentsGroups[index] = new DevelopmentGroup(
                    Constants.Packages.devcards[index + ((index > 0) ? 1 : 0)]);

            flow.getChildren().add(developmentsGroups[index]);
        }

        local.addOnInventoryChange(() -> {
            byte[] mats = local.getMaterials();
            for (byte index = (byte) 0; index < mats.length; index++) {
                materialGroups[index].setCount(mats[index]);
            }
        });

    }
}
