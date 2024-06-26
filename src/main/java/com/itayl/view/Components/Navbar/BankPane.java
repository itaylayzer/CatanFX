package com.itayl.view.Components.Navbar;

import com.itayl.view.Constants;
import com.itayl.view.Constants.Materials;

import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;

public class BankPane extends VBox {

    // components
    private MiniMaterialGroup woodGroup;
    private MiniMaterialGroup woolGroup;
    private MiniMaterialGroup wheetGroup;
    private MiniMaterialGroup brickGroup;
    private MiniMaterialGroup oreGroup;
    private MiniMaterialGroup devGroup;
    private Label title;

    private FlowPane flowPane;
    private StackPane stackPane;

    private final double fontSize = 20;

    public BankPane() {

        this.flowPane = new FlowPane();
        this.title = new Label("Bank");
        woodGroup = new MiniMaterialGroup(Constants.Packages.materials[0]);
        woolGroup = new MiniMaterialGroup(Constants.Packages.materials[1]);
        wheetGroup = new MiniMaterialGroup(Constants.Packages.materials[2]);
        brickGroup = new MiniMaterialGroup(Constants.Packages.materials[3]);
        oreGroup = new MiniMaterialGroup(Constants.Packages.materials[4]);
        devGroup = new MiniMaterialGroup(Constants.Packages.devcard);

        woodGroup.setScaleOnHover(false);
        woolGroup.setScaleOnHover(false);
        wheetGroup.setScaleOnHover(false);
        brickGroup.setScaleOnHover(false);
        oreGroup.setScaleOnHover(false);
        devGroup.setScaleOnHover(false);

        flowPane.setHgap(3.1);
        flowPane.getChildren().add(woodGroup);
        flowPane.getChildren().add(woolGroup);
        flowPane.getChildren().add(wheetGroup);
        flowPane.getChildren().add(brickGroup);
        flowPane.getChildren().add(oreGroup);
        flowPane.getChildren().add(devGroup);

        stackPane = new StackPane();
        stackPane.getChildren().add(flowPane);

        super.getChildren().add(initTitle());
        super.getChildren().add(stackPane);
    }

    private Group initTitle() {
        title.setFont(Font.font("Monospace", FontWeight.BOLD,
                FontPosture.REGULAR, fontSize));
        title.setTextFill(Color.WHITE);
        title.setLayoutX(10);

        Rectangle labelRect = new Rectangle();
        labelRect.setWidth(280);
        labelRect.setHeight(fontSize * 1.2);
        labelRect.setFill(Color.GOLD);
        labelRect.setArcHeight(10);
        labelRect.setArcWidth(10);
        labelRect.setOpacity(0.5);

        Group labelGroup = new Group();
        labelGroup.getChildren().add(labelRect);
        labelGroup.getChildren().add(title);
        return labelGroup;
    }

    public void setMaterials(byte[] materials) {

        for (int matIndex = 0; matIndex < materials.length; matIndex++) {
            setCount(Materials.values()[matIndex], materials[matIndex]);
        }
    }

    public void setCount(Materials material, byte count) {
        MiniMaterialGroup[] materialGroups = new MiniMaterialGroup[] {
                woodGroup,
                woolGroup,
                wheetGroup,
                brickGroup,
                oreGroup
        };
        materialGroups[material.getValue()].setCount(count);
    }

    public void setDevelopements(byte[] devs) {
        byte sum = 0;
        for (int i = 0; i < devs.length; i++) {
            sum += devs[i];
        }
        devGroup.setCount(sum);
    }
}
