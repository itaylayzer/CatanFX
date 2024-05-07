package com.c1t45.view.Inventory;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Player;
import com.c1t45.view.CatanBoard.CatanBoard;
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

        for (byte index = (byte) 0; index < (byte) materialGroups.length; index++) {
            materialGroups[index] = new MaterialGroup(Constants.Packages.materials[index]);
            flow.getChildren().add(materialGroups[index]);
        }

        developmentsGroups = new DevelopmentGroup[] {
                // knight
                new DevelopmentGroup(Constants.Packages.devcards[0], (card) -> {
                    CatanBoard board = CatanBoard.getInstance();
                    card.setButtonDisabled(true);
                    board.cancelCurrentPick(false);

                    board.pickHexagon((value) -> {
                        return value != 9 && board.getRobberPos() != value;
                    }, () -> {
                        card.setButtonDisabled(false);
                    }, (picked) -> {
                        card.setButtonDisabled(false);
                        System.out.println("picked-hexagon=" + picked);
                        board.setRobberPos(picked);
                        localPlayer.usedDevCard((byte) 0);
                    });
                }),
                new DevelopmentGroup(Constants.Packages.devcards[1]),
                // roads
                new DevelopmentGroup(Constants.Packages.devcards[2], (card) -> {
                    CatanBoard board = CatanBoard.getInstance();
                    card.setButtonDisabled(true);
                    board.cancelCurrentPick(false);

                    board.pickEdge((value) -> {
                        return Player.roadDontBelong(value) && localPlayer.validRoadsToBuy().contains(value);
                    }, () -> {
                        card.setButtonDisabled(false);
                    }, (picked) -> {

                        System.out.println("picked-edge=" + picked);
                        byte[] fromto = board.seperateEdge(picked);
                        localPlayer.buyRoad(false, picked, fromto[0], fromto[1]);
                        CatanBoard.addRoad(picked, localPlayer.getColor());
                        card.setButtonDisabled(true);
                        // second pick
                        board.pickEdge((value) -> {
                            return Player.roadDontBelong(value) && localPlayer.validRoadsToBuy().contains(value);
                        }, () -> {
                            card.setButtonDisabled(false);
                        }, (picked_second) -> {
                            System.out.println("picked-edge=" + picked_second);
                            byte[] fromto2 = board.seperateEdge(picked_second);
                            localPlayer.buyRoad(false, picked, fromto2[0], fromto2[1]);
                            CatanBoard.addRoad(picked, localPlayer.getColor());
                            localPlayer.usedDevCard((byte) 2);
                        }, true, false);
                    }, true, true);
                }),
                // year of plant
                new DevelopmentGroup(Constants.Packages.devcards[3], (card) -> {
                    // TODO: two selection alert! of mats!
                    localPlayer.usedDevCard((byte) 3);
                }),
                // monopol
                new DevelopmentGroup(Constants.Packages.devcards[4], (card) -> {
                    // TODO: selection alert! of mats!
                    localPlayer.usedDevCard((byte) 4);
                }),
        };

        for (byte index = (byte) (0); index < developmentsGroups.length; index++) {
            flow.getChildren().add(developmentsGroups[index]);
        }

        local.addOnInventoryChange(() -> {
            byte[] mats = local.getMaterials();
            byte[] devcards = local.getDevelopments();
            for (byte index = (byte) 0; index < mats.length; index++) {
                materialGroups[index].setCount(mats[index]);
            }

            for (byte index = (byte) 0; index < devcards.length; index++) {
                developmentsGroups[index].setCount(devcards[index]);
                developmentsGroups[index].setButtonDisabled(false);
            }
        });

    }
}
