package com.itayl.view.Components.Selection;

import com.itayl.view.Constants;
import com.itayl.view.Player;
import com.itayl.view.Components.Buttons.SelectButton;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Packages.ImagePackage;

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
    private byte allowed;

    public PlayerSelection(byte allowed, Player[] players, Color[] colors) {
        this.colors = colors;
        this.players = players;
        this.allowed = allowed;
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

            if ((this.allowed >> index & 0x01) == 0)
                continue;
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
