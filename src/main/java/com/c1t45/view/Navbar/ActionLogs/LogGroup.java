package com.c1t45.view.Navbar.ActionLogs;

import com.c1t45.model.Player;

import javafx.geometry.Insets;
import javafx.scene.Group;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;

public abstract class LogGroup extends Group {

    // components
    private Label nameLabel;
    private Label logLabel;
    private ImageView imageView;

    // containers
    private HBox hBox;
    private Rectangle rect;

    public LogGroup(Player player, String logString) {
        nameLabel = new Label(player.getName().toUpperCase());
        nameLabel.setFont(Font.font("Segoe UI", FontWeight.BOLD, FontPosture.ITALIC, 10));
        nameLabel.setTextFill(Color.WHITE);
        nameLabel.setOpacity(0.7);
        nameLabel.setTranslateY(4);

        imageView = new ImageView();
        this.logLabel = initLogLabel(logString);

        final double padding = 5;

        hBox = new HBox();
        hBox.setPadding(new Insets(0, padding, 0, padding));
        hBox.setSpacing(padding);
        hBox.getChildren().add(nameLabel);
        hBox.getChildren().add(logLabel);
        hBox.getChildren().add(imageView);
        HBox.setHgrow(logLabel, Priority.ALWAYS);

        rect = new Rectangle();
        rect.setFill(player.getColor());
        rect.setArcHeight(20);
        rect.setArcWidth(20);
        rect.setWidth(305);
        rect.setHeight(20);
        rect.setOpacity(0.5);

        getChildren().add(rect);
        getChildren().add(hBox);

    }

    public void setImage(Image image, double height, double width) {
        this.imageView.setImage(image);
        this.imageView.setFitHeight(height);
        this.imageView.setFitWidth(width);
    }

    public Label initLogLabel(String logString) {
        String log = logString;
        Label label = new Label(log);
        label.setFont(Font.font("Segoe UI", FontWeight.NORMAL, FontPosture.REGULAR, 14));
        label.setTextFill(Color.WHITE);
        return label;
    }

    public ImageView getImageView() {
        return this.imageView;
    }
}
