package com.c1t45.controller;

import com.c1t45.view.Constants;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class MenuController {
    @FXML
    private TextField nameField;
    @FXML
    private Label countLabel;
    @FXML
    private Slider countSlider;
    @FXML
    private Button startButton;

    private byte playerCount = 1;

    public void initialize() {
        startButton.setOnAction((event) -> {
            try {
                // locale url to the class
                FXMLLoader loader = new FXMLLoader(getClass().getResource("../resources/fxml/game.fxml"));
                Parent root = loader.load();

                GameController gameController = (GameController) loader.getController();
                Stage stage = (Stage) ((Node) event.getSource()).getScene().getWindow();
                gameController.initialize(stage, playerCount, nameField.getText());
                Scene scene = new Scene(root);

                stage.setTitle("Catan: Trade Build Settle");
                stage.setScene(scene);

                // this url is locale to the class
                root.getStylesheets().add(getClass().getResource("../resources/css/main.css").toExternalForm());
                // this url is locale to the stage
                stage.getIcons().add(Constants.Images.icon);
                stage.show();
            } catch (Exception e) {
                e.printStackTrace();
            }

        });
        countSlider.valueProperty().addListener((observable, oldValue, newValue) -> {
            playerCount = newValue.byteValue();
            countLabel.setText(Integer.toString(newValue.intValue()));
        });
    }

}
