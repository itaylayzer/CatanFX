package com.c1t45.view.Components.Navbar;

import com.c1t45.view.Constants;
import com.c1t45.view.Player;

import javafx.geometry.Insets;
import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;

class Tag extends Group {
    public Tag(String string) {
        Label label = new Label(string);
        label.setFont(Font.font("Segoe UI", FontWeight.BOLD, FontPosture.REGULAR, 15));
        label.setTextFill(Color.WHITE);
        label.setLayoutY(5);
        label.setLayoutX(25);

        Rectangle rect = new Rectangle();
        rect.setWidth(150);
        rect.setHeight(30);
        rect.setArcHeight(10);
        rect.setArcWidth(10);

        rect.setFill(Paint.valueOf("#0d0d0d"));

        getChildren().add(rect);
        getChildren().add(label);

    }
}

class TagStack extends StackPane {

    private HBox hbox;

    public TagStack() {
        hbox = new HBox();

        hbox.setSpacing(10);
        setMargin(hbox, new Insets(4, 0, 0, 0));

        getChildren().add(hbox);
    }

    public void add(Group group) {
        hbox.getChildren().add(group);

        switch (hbox.getChildren().size()) {
            case 1:
                hbox.setMinWidth(150);
                hbox.setMaxWidth(150);
                break;

            case 2:
                hbox.setMinWidth(310);
                hbox.setMaxWidth(310);
                break;
        }
        ;
    }

    public void remove(Group group) {
        hbox.getChildren().remove(group);
    }

    public boolean contains(Group group) {
        return hbox.getChildren().contains(group);
    }
}

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
    private Tag longestRoad, biggestArmy;
    private TagStack tagStack;

    public PlayerPane(Player player) {

        this.player = player;
        this.flowPane = new FlowPane();
        this.title = new Label(player.getName());
        this.materialsGroup = new StatsGroup(Constants.Packages.material);
        this.devcardsGroup = new StatsGroup(Constants.Packages.devcard);

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

        tagStack = new TagStack();

        biggestArmy = new Tag("Biggest Army");
        longestRoad = new Tag("Longest Road");

        super.getChildren().add(initTitle());
        super.getChildren().add(stackPane);
        super.getChildren().add(tagStack);

        player.addOnInventoryChange(() -> {
            updateInfo();
        });

        Player.addOnAchievementsChange((a, b) -> {
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

    public void setLongestRoad(boolean b) {
        if (b) {
            if (!tagStack.contains(longestRoad))
                tagStack.add(longestRoad);
        } else {
            if (tagStack.contains(longestRoad))
                tagStack.remove(longestRoad);
        }
    }

    public void setBigestArmy(boolean b) {
        if (b) {
            if (!tagStack.contains(biggestArmy))
                tagStack.add(biggestArmy);
        } else {
            if (tagStack.contains(biggestArmy))
                tagStack.remove(biggestArmy);
        }
    }
}
