package com.itayl.view.Windows;

import com.itayl.view.Constants;
import com.itayl.view.Components.Buttons.CountButton;
import com.itayl.view.Components.Buttons.SelectButton;

import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Paint;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

public class TradeWindow extends Stage {
    final static int width = 500;

    public TradeWindow() {
        VBox box = new VBox();
        box.setStyle("-fx-background-color: #1a1a1a");
        box.setPadding(new Insets(20));
        box.setSpacing(10);

        HBox buttons = new HBox();
        buttons.setSpacing(10);

        SelectButton cancel = new SelectButton(Constants.Packages.cancel, true);
        SelectButton bank = new SelectButton(Constants.Packages.harbor, true);
        SelectButton harbor = new SelectButton(Constants.Packages.bank, true);

        buttons.getChildren().add(cancel);
        buttons.getChildren().add(bank);
        buttons.getChildren().add(harbor);

        BorderPane pane = new BorderPane(buttons);

        FlowPane top = new FlowPane();
        FlowPane bottom = new FlowPane();

        Label topLabel = new Label("recieve");
        Label bottomLabel = new Label("send");

        topLabel.setFont(Font.font("consolas", FontWeight.BLACK, FontPosture.ITALIC, 18));
        topLabel.setTextFill(Paint.valueOf("#c0c0c0"));

        bottomLabel.setFont(Font.font("consolas", FontWeight.BLACK, FontPosture.ITALIC, 18));
        bottomLabel.setTextFill(Paint.valueOf("#c0c0c0"));

        top.getChildren().add(topLabel);
        bottom.getChildren().add(bottomLabel);

        for (int index = 0; index < Constants.Packages.materials.length; index++) {
            var pack = Constants.Packages.materials[index];
            CountButton _topMat = new CountButton(pack, 0.5);
            CountButton _bottomMat = new CountButton(pack, 0.5);

            top.getChildren().add(_topMat);
            bottom.getChildren().add(_bottomMat);
        }

        box.getChildren().add(top);
        box.getChildren().add(pane);
        box.getChildren().add(bottom);

        setTitle("Trade");
        Scene scene = new Scene(box);

        setResizable(false);
        setScene(scene);

        pane.setMinHeight(getHeight());
        pane.setMinWidth(width);
        pane.setMaxWidth(width);
        pane.setPrefWidth(width);

        getIcons().add(Constants.Images.icon);
        setWidth(width);
    }
}
