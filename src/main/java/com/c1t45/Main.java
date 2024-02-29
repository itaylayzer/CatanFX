package com.c1t45;

import com.c1t45.controller.GameController;
import com.c1t45.view.Constants;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(@SuppressWarnings("exports") Stage primaryStage) throws Exception {
        startGame(primaryStage);
        primaryStage.setMinHeight(100);
        primaryStage.setMinWidth(1203);
    }

    void startMenu(Stage primaryStage) {
        try {
            Parent root = FXMLLoader.load(getClass().getResource("fxml/menu.fxml"));

            primaryStage.setTitle("Catan: Trade Build Settle");

            primaryStage.setScene(new Scene(root));

            root.getStylesheets().add(getClass().getResource("css/main.css").toExternalForm());

            primaryStage.getIcons().add(Constants.Images.icon);

            primaryStage.show();

            primaryStage.setMinHeight(639);
            primaryStage.setMinWidth(816);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void startGame(Stage primaryStage) {
        try {
            new Image(getClass().getResource("icons/materials/wood3.png").toString());

            FXMLLoader loader = new FXMLLoader(getClass().getResource("fxml/game.fxml"));

            Parent root = loader.load();
            GameController gameController = (GameController) loader.getController();

            gameController.initialize(primaryStage, (byte) 4, "Local Runner");
            Scene scene = new Scene(root);

            primaryStage.setTitle("Catan: Trade Build Settle");
            primaryStage.setScene(scene);

            root.getStylesheets().add(getClass().getResource("css/main.css").toExternalForm());
            primaryStage.getIcons().add(Constants.Images.icon);
            primaryStage.show();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
