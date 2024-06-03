package com.itayl.view.Windows;

import com.itayl.view.Constants;
import com.itayl.view.Components.Buttons.ImageButton;
import com.itayl.view.Packages.ImagePackage;
import com.itayl.view.Utils.ImageUtils;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;
import javafx.scene.text.TextAlignment;
import javafx.stage.Stage;

class StorePackage {
    final public String name;
    final public byte[] prices;
    final public Image image;

    public StorePackage(String name, byte[] price, Image image) {
        this.name = name;
        this.prices = price;
        this.image = image;
    }

}

class MaterialGroup extends ImageButton {
    private static final double scale = 1.3;

    public MaterialGroup(ImagePackage pack, byte count) {
        super(50 / scale, 90 / scale, 5 / scale);

        setFontSize(24);
        setText(Byte.toString(count));

        setFill(Paint.valueOf("#1a1a1a"));
        setInnerFill(pack.color);

        setImage(pack.image);
        setTooltip(pack.name);
    }
}

public class StoreWindow extends Stage {

    static private final double height = 700;
    static private final double width = 500;
    private VBox box;

    public StoreWindow() {

        box = new VBox();
        box.setStyle("-fx-background-color: #1a1a1a");
        box.setPadding(new Insets(20));
        box.setSpacing(10);

        initialize();

        setTitle("Catan Store");
        Scene scene = new Scene(box);

        setWidth(width);
        setHeight(height);
        setResizable(false);
        setScene(scene);

        getIcons().add(Constants.Images.icon);
    }

    private void initialize() {
        Label title = new Label("STORE");
        title.setFont(Font.font("Consolas", FontWeight.BLACK, FontPosture.ITALIC, 35));

        title.setTextFill(Color.WHITE);
        title.setAlignment(Pos.CENTER);
        title.setTextAlignment(TextAlignment.CENTER);
        title.setMaxWidth(width - 20);

        box.getChildren().add(title);

        for (StorePackage pack : new StorePackage[] {
                new StorePackage("Road", Constants.Store.road, Constants.Images.roads),
                new StorePackage("House", Constants.Store.house, Constants.Images.house),
                new StorePackage("City", Constants.Store.city, Constants.Images.city),
                new StorePackage("Development card", Constants.Store.developmentCard, Constants.Images.devcards),
        }) {
            Group itemGroup = new Group();
            VBox itemBox = new VBox();
            Rectangle rect = new Rectangle();
            rect.setFill(Paint.valueOf("#282828"));
            rect.setWidth(width - 55);
            rect.setArcHeight(30);
            rect.setArcWidth(30);
            rect.setHeight(120);
            HBox priceBox = new HBox();

            FlowPane materialsPane = new FlowPane();
            materialsPane.setHgap(10);
            ImageView itemView = new ImageView(ImageUtils.colorChange(pack.image));
            itemView.setFitHeight(50);
            itemView.setFitWidth(50);

            Label nameLabel = new Label(pack.name.toLowerCase());
            nameLabel.setFont(Font.font("Segoe UI", FontWeight.NORMAL, FontPosture.REGULAR, 25));
            nameLabel.setTextFill(Color.WHITE);

            byte index;
            for (index = 0; index < pack.prices.length; index++) {
                byte price = pack.prices[index];
                if (price > 0) {
                    MaterialGroup imageButton = new MaterialGroup(Constants.Packages.materials[index], price);
                    materialsPane.getChildren().add(imageButton);
                }
            }

            Group itemImageGroup = new Group();

            Rectangle imageRect = new Rectangle();

            imageRect.setWidth(index);
            imageRect.setFill(Paint.valueOf("#1a1a1a"));
            imageRect.setWidth(50);
            imageRect.setWidth(50);
            imageRect.setArcHeight(25);
            imageRect.setArcWidth(25);

            itemImageGroup.getChildren().add(imageRect);
            itemImageGroup.getChildren().add(itemView);

            HBox.setMargin(itemImageGroup, new Insets(0, 0, 0, -50));

            priceBox.getChildren().add(materialsPane);
            priceBox.getChildren().add(itemImageGroup);

            HBox.setHgrow(materialsPane, Priority.ALWAYS);

            itemBox.getChildren().add(nameLabel);
            itemBox.getChildren().add(priceBox);

            itemBox.setPadding(new Insets(10));
            itemGroup.getChildren().add(rect);
            itemGroup.getChildren().add(itemBox);
            box.getChildren().add(itemGroup);

        }
    }
}
