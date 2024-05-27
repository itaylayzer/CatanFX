package com.c1t45;

import java.io.IOException;
import java.io.InputStream;
import java.net.UnknownHostException;
import java.util.Scanner;

import com.c1t45.controller.GameController;
import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Components.CatanBoard.CatanBoard;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    static Scanner scanner = new Scanner(System.in);

    private void open(Stage primaryStage) throws Exception, UnknownHostException, IOException {
        startGame(primaryStage);
        primaryStage.setMinHeight(720);
        primaryStage.setMinWidth(1330);
    }

    @Override
    public void start(@SuppressWarnings("exports") Stage primaryStage) {
        try {
            open(primaryStage);

        } catch (Exception ex) {
            ex.printStackTrace(System.err);
        }
    }

    public static void waitForInput() {

        System.out.println("Press any key to continue...");
        scanner.nextLine(); // Wait for the user to press Enter
    }

    void restartGame(Stage primaryStage) {
        primaryStage.close();
        try {
            CatanBoard.clear();
            LocalPlayer.clear();
            waitForInput();
            open(new Stage());
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    void startMenu(Stage primaryStage) {
        try {
            Parent root = FXMLLoader.load(getClass().getResource("fxml/menu.fxml"));
            primaryStage.setTitle("Catan: Trade Build Settle");

            primaryStage.setScene(new Scene(root));
            root.getStylesheets().add(getClass().getResource("css/main.css").toExternalForm());

            primaryStage.getIcons().add(Constants.Images.icon);
            primaryStage.show();

            primaryStage.setMinHeight(739);
            primaryStage.setMinWidth(1356);

        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    public byte[] getEdges() throws IOException {
        String filePath = "files/edges.byte";
        InputStream inputStream = getClass().getResourceAsStream(filePath);

        if (inputStream == null) {
            throw new IOException("File not found: " + filePath);
        }

        byte[] buffer = new byte[inputStream.available()];
        inputStream.read(buffer);
        inputStream.close();
        return buffer;

    }

    void startGame(Stage primaryStage) throws Exception, UnknownHostException, IOException {

        FXMLLoader loader = new FXMLLoader(getClass().getResource("fxml/game.fxml"));

        Parent root = loader.load();
        GameController gameController = (GameController) loader.getController();

        primaryStage.setOnCloseRequest(event -> {
            event.consume();
            primaryStage.close();
            restartGame(primaryStage);
        });

        byte[] edges = getEdges();
        gameController.initialize(primaryStage, (byte) 3, "Local Runner", edges);
        Scene scene = new Scene(root);

        primaryStage.setTitle("Catan: Trade Build Settle");
        primaryStage.setScene(scene);

        root.getStylesheets().add(getClass().getResource("css/main.css").toExternalForm());
        primaryStage.getIcons().add(Constants.Images.icon);
        primaryStage.show();

    }

    public static void main(String[] args) {
        launch(args);
    }
}
