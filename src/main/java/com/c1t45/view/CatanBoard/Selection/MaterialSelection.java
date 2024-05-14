package com.c1t45.view.CatanBoard.Selection;

import com.c1t45.view.Constants;
import com.c1t45.view.Components.SelectButton;
import com.c1t45.view.Interfaces.Action;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.layout.VBox;

public class MaterialSelection extends AbstractSelection {

    @Override
    public void selection(VBox box, com.c1t45.view.Interfaces.Action<Byte> onFinish, boolean cancellable) {
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

        for (int index = 0; index < Constants.Packages.materials.length; index++) {

            ObjectProperty<Integer> integerIndex = new SimpleObjectProperty<>(index);
            SelectButton materialButton = new SelectButton(Constants.Packages.materials[index], 0.5);

            materialButton.setOnAction((e) -> {
                disposeChildren.action(null);
                onFinish.action(integerIndex.getValue().byteValue());
            });

            children.add(materialButton);
        }
    }

}
