package com.c1t45.view.Components.Selection;

import com.c1t45.view.Constants;
import com.c1t45.view.Player;
import com.c1t45.view.Components.Buttons.SelectButton;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Packages.ImagePackage;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;

public class PlayerSelection extends AbstractSelection {

    private Color[] colors;
    private Player[] players;

    public PlayerSelection(Player[] players, Color[] colors) {
        this.colors = colors;
        this.players = players;
    }

    @Override
    public void selection(VBox box, Action<Byte> onFinish, boolean cancellable) {
        VBox container = new VBox();
        container.setPadding(new Insets(10));
        container.setStyle("-fx-background-color:#1a1a1a");
        container.setSpacing(10);
        box.getChildren().add(container);

        ObservableList<Node> children = container.getChildren();
        Action<Void> disposeChildren = (v) -> {
            children.clear();
            box.getChildren().remove(container);
        };

        if (cancellable) {
            SelectButton cancelButton = new SelectButton(Constants.Packages.cancel, true);

            cancelButton.setOnAction((e) -> {
                disposeChildren.action(null);
                onFinish.action((byte) -1);
            });

            children.add(cancelButton);
        }

        for (int index = 1; index < this.players.length; index++) {

            ObjectProperty<Integer> integerIndex = new SimpleObjectProperty<>(index);
            SelectButton materialButton = new SelectButton(
                    new ImagePackage(players[index].getName(), colors[index], Constants.Images.player), 0.5);

            materialButton.setOnAction((e) -> {
                disposeChildren.action(null);

                onFinish.action(integerIndex.getValue().byteValue());
            });

            children.add(materialButton);
        }
    }

}
