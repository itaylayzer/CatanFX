package com.c1t45.view.Inventory;

import com.c1t45.view.Constants;
import com.c1t45.view.Components.MaterialGroup;

import javafx.scene.layout.FlowPane;

public class InventoryPane {

    FlowPane flow;
    MaterialGroup[] materialGroups;

    public InventoryPane(FlowPane inventoryFlow) {
        flow = inventoryFlow;
        materialGroups = new MaterialGroup[5];

        for (byte index = (byte) 0; index < (byte) 5; index++) {
            materialGroups[index] = new MaterialGroup(Constants.Packages.materials[index]);
            flow.getChildren().add(materialGroups[index]);
        }
    }

    public void Render(byte[] materialList) {
        for (byte index = (byte) 0; index < materialList.length; index++) {
            materialGroups[index].setCount(materialList[index]);
        }
    }
}
