package com.itayl.view.Components.Counts;

import com.itayl.view.Constants;
import com.itayl.view.Components.Buttons.CountButton;
import com.itayl.view.Components.Buttons.SelectButton;
import com.itayl.view.Interfaces.Action;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;

public class MaterialCounts {
    public void startCounting(VBox box, Action<Byte[]> onFinish, byte[] maxCounts, byte count) {

        ObjectProperty<Byte[]> propertyMaterials = new SimpleObjectProperty<>(new Byte[] { 0, 0, 0, 0, 0 });
        ObjectProperty<Byte> amount = new SimpleObjectProperty<>((byte) 0);
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

        SelectButton approveButton = new SelectButton(Constants.Packages.approve, true);
        approveButton.setButtonDisabled(true);
        Label label = new Label(count + " LEFT");
        label.setFont(Font.font("consolas", FontWeight.LIGHT, FontPosture.ITALIC, 20));
        children.add(label);

        propertyMaterials.addListener((obs, old, newValue) -> {
            byte sum = 0;
            for (byte c : newValue) {
                sum += c;
            }
            amount.setValue((byte) sum);

        });

        amount.addListener((obs, old, newValue) -> {
            label.setText((count - newValue) + " LEFT");
            approveButton.setButtonDisabled(count != newValue);
        });
        for (int index = 0; index < Constants.Packages.materials.length; index++) {

            ObjectProperty<Integer> integerIndex = new SimpleObjectProperty<>(index);
            CountButton materialButton = new CountButton(Constants.Packages.materials[index], 0.5);

            materialButton.setOnCountAction(param -> {
                byte aIndex = integerIndex.get().byteValue();

                if (param > maxCounts[aIndex]) {
                    materialButton.setCount(maxCounts[aIndex]);
                    param = maxCounts[aIndex];
                }

                Byte[] oldVal = propertyMaterials.get();
                Byte[] newVal = oldVal.clone();
                newVal[aIndex] = param;
                propertyMaterials.setValue(newVal);
            });

            children.add(materialButton);
        }

        approveButton.setOnAction((e) -> {
            disposeChildren.action(null);
            onFinish.action(propertyMaterials.getValue());
        });

        children.add(approveButton);
    }
}
