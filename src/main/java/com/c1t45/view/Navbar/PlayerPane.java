package com.c1t45.view.Navbar;

import com.c1t45.model.Player;
import com.c1t45.view.Constants;

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

public class PlayerPane extends VBox {

    // variables
    private Player player;

    // components
    private StatsGroup materialsGroup;
    private StatsGroup devcardsGroup;
    private StatsGroup pointsGroup;
    private StatsGroup roadsGroup;
    private Label title;

    private FlowPane flowPane;
    private StackPane stackPane;

    private final double fontSize = 20;

    public PlayerPane(Player player) {

        this.player = player;
        this.flowPane = new FlowPane();
        this.title = new Label(player.getName());
        this.materialsGroup = new StatsGroup(Constants.Packages.material);
        this.devcardsGroup = new StatsGroup(Constants.Packages.devcards);

        this.pointsGroup = new StatsGroup(Constants.Packages.points);
        this.roadsGroup = new StatsGroup(Constants.Packages.roads);

        materialsGroup.setScaleOnHover(false);
        devcardsGroup.setScaleOnHover(false);
        pointsGroup.setScaleOnHover(false);
        roadsGroup.setScaleOnHover(false);

        flowPane.setHgap(10);
        flowPane.getChildren().add(identifierRectangle());
        flowPane.getChildren().add(materialsGroup);
        flowPane.getChildren().add(devcardsGroup);
        flowPane.getChildren().add(pointsGroup);
        flowPane.getChildren().add(roadsGroup);

        stackPane = new StackPane();
        stackPane.getChildren().add(flowPane);

        super.getChildren().add(initTitle());
        super.getChildren().add(stackPane);
        player.addOnInventoryChange(() -> {
            updateInfo();
        });
        updateInfo();

    }

    private void updateInfo() {
        materialsGroup.setCount(player.getMaterialsCount());
        devcardsGroup.setCount(player.getDevCardsCount());
        pointsGroup.setCount(player.getVicotryPoints());
        roadsGroup.setCount(player.getLongestRoadLength());
    }

    private Rectangle identifierRectangle() {
        Rectangle identifierRect = new Rectangle();
        identifierRect.setWidth(5);
        identifierRect.setHeight(45);
        identifierRect.setFill(player.getColor());
        identifierRect.setArcHeight(5);
        identifierRect.setArcWidth(5);
        identifierRect.setScaleY(0.6);
        identifierRect.setScaleX(0.7);
        return identifierRect;
    }

    private Group initTitle() {
        title.setFont(Font.font("Monospace", FontWeight.BOLD,
                FontPosture.REGULAR, fontSize));
        title.setTextFill(Color.WHITE);
        title.setLayoutX(10);

        Rectangle labelRect = new Rectangle();
        labelRect.setWidth(330);
        labelRect.setHeight(fontSize * 1.2);
        labelRect.setFill(player.getColor());
        labelRect.setArcHeight(10);
        labelRect.setArcWidth(10);
        labelRect.setOpacity(0.5);

        Group labelGroup = new Group();
        labelGroup.getChildren().add(labelRect);
        labelGroup.getChildren().add(title);
        return labelGroup;
    }
}
