package com.itayl.view.Windows;

import com.itayl.view.Constants;
import com.itayl.view.LocalPlayer;
import com.itayl.view.Components.Buttons.CountButton;
import com.itayl.view.Components.Buttons.SelectButton;
import com.itayl.view.Interfaces.Action;

import com.itayl.view.Utils.BytesUtils;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
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

    public TradeWindow(LocalPlayer player, Action<Byte> onTrade, Action<TradeWindow> onClose) {
        byte[] mats = player.getMaterials();

        byte[] send = new byte[] { (byte) 0, (byte) 0, (byte) 0, (byte) 0, (byte) 0 };
        byte[] recieve = new byte[] { (byte) 0, (byte) 0, (byte) 0, (byte) 0, (byte) 0 };

        VBox box = new VBox();
        TradeWindow self = this;
        box.setStyle("-fx-background-color: #1a1a1a");
        box.setPadding(new Insets(20));
        box.setSpacing(10);

        HBox buttons = new HBox();
        buttons.setSpacing(10);

        SelectButton cancel = new SelectButton(Constants.Packages.cancel, true);
        cancel.setOnAction((event) -> {
            onClose.action(self);
        });
        SelectButton bank = new SelectButton(Constants.Packages.bank, true);
        SelectButton harbor = new SelectButton(Constants.Packages.harbor, true);

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

        Action<Void> update = (v) -> {

            byte two = self.harborTradeable(player.getHarbors(), self.tradable(send, recieve, (byte) 2));

            byte three = (byte) (self.tradable(send, recieve, (byte) 3) *
                    ((player.getHarbors() >> 5) & 0x01));

            byte four = self.tradable(send, recieve, (byte) 4);

            harbor.setButtonDisabled(three == 0 && two == 0);
            bank.setButtonDisabled(four == 0);

            bank.setOnAction((event) -> {
                onTrade.action(self.tradable(send, recieve, (byte) 4));
                onClose.action(self);
            });
            harbor.setOnAction((event) -> {
                onTrade.action(
                        BytesUtils.max(self.tradable(send, recieve, (byte) 2), self.tradable(send, recieve, (byte) 3)));
                onClose.action(self);

            });
        };

        for (int index = 0; index < Constants.Packages.materials.length; index++) {
            var pack = Constants.Packages.materials[index];
            CountButton _topMat = new CountButton(pack, 0.5);
            CountButton _bottomMat = new CountButton(pack, 0.5);

            ObjectProperty<Integer> indexObj = new SimpleObjectProperty<Integer>(index);
            _topMat.setOnCountAction((event) -> {
                recieve[indexObj.get()] = event;
                update.action(null);
            });
            _bottomMat.setOnCountAction((event) -> {
                int aIndex = indexObj.get();
                if (event > mats[aIndex]) {
                    _bottomMat.setCount(mats[aIndex]);
                    event = mats[aIndex];
                }
                send[aIndex] = event;
                update.action(null);
            });

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

        this.setOnCloseRequest((event) -> {
            onClose.action(self);
        });

    }

    private byte harborTradeable(byte harbors, byte tradable) {
        byte send_offset = (byte) ((tradable & 0x07));
        return (byte) (tradable * ((harbors >> (send_offset - 1)) & 0x01));
    }

    private byte tradable(byte[] send, byte[] request, byte tradeoff) {
        byte request_offset = (byte) (BytesUtils.find(request, (byte) 1) + 1);
        byte send_offset = (byte) (BytesUtils.find(send, tradeoff) + 1);
        if (send_offset * request_offset == 0)
            return 0;

        byte together = (byte) (send_offset + (request_offset << 3));
        System.out.println("send_offset=" + send_offset + " request_offset=" + request_offset
                + " togehter" + together);
        return together;
    }

}
