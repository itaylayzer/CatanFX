package com.c1t45.view.Components.Inventory;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Player;
import com.c1t45.view.Components.CatanBoard.CatanBoard;
import com.c1t45.view.Components.Groups.DevelopmentGroup;
import com.c1t45.view.Components.Groups.MaterialGroup;

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
                        localPlayer.moveRobber(picked, (byte) 0);
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
                    }, (picked_first) -> {

                        System.out.println("picked-edge=" + picked_first);
                        byte[] fromto = board.seperateEdge(picked_first);
                        localPlayer.buyRoad(false, picked_first, fromto[0], fromto[1]);
                        CatanBoard.addRoad(picked_first, localPlayer.getColor());
                        card.setButtonDisabled(true);
                        // second pick
                        board.pickEdge((value) -> {
                            return Player.roadDontBelong(value) && localPlayer.validRoadsToBuy().contains(value);
                        }, () -> {
                            card.setButtonDisabled(false);
                        }, (picked_second) -> {
                            System.out.println("picked-edge=" + picked_second);
                            byte[] fromto2 = board.seperateEdge(picked_second);
                            localPlayer.buyRoad(false, picked_second, fromto2[0], fromto2[1]);
                            CatanBoard.addRoad(picked_second, localPlayer.getColor());
                            localPlayer.usedDevCard((byte) 2);
                        }, true, false);
                    }, true, true);
                }),
                // year of plant
                new DevelopmentGroup(Constants.Packages.devcards[3], (card) -> {
                    CatanBoard board = CatanBoard.getInstance();
                    card.setButtonDisabled(true);
                    board.cancelCurrentPick(false);
                    board.materialSelect((firstMat) -> {
                        if (firstMat < 0) {
                            card.setButtonDisabled(false);

                        } else
                            board.materialSelect((secondMat) -> {
                                if (secondMat >= 0) {
                                    localPlayer.yearOfPlant(firstMat, secondMat);
                                }
                                card.setButtonDisabled(false);

                            }, true);
                    }, true);

                }),
                // monopol
                new DevelopmentGroup(Constants.Packages.devcards[4], (card) -> {
                    CatanBoard board = CatanBoard.getInstance();
                    card.setButtonDisabled(true);
                    board.cancelCurrentPick(false);
                    board.materialSelect((matIndex) -> {
                        if (matIndex >= 0) {
                            localPlayer.monopol(matIndex);
                        }
                        card.setButtonDisabled(false);
                    }, true);
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
