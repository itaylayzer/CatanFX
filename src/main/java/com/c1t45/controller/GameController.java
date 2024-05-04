package com.c1t45.controller;

import com.c1t45.view.EnemyPlayer;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Player;
import com.c1t45.view.UserInterface;
import com.c1t45.view.CatanBoard.CatanBoard;
import com.c1t45.view.CatanBoard.DicePane;
import com.c1t45.view.CatanBoard.NotificationGroup;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Navbar.PlayersFlow;
import com.c1t45.view.Packages.NotificationPackage;
import com.c1t45.view.Windows.StoreWindow;

import javafx.animation.AnimationTimer;
import javafx.animation.Interpolator;
import javafx.animation.ScaleTransition;
import javafx.application.Platform;
import javafx.beans.binding.Bindings;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Window;
import javafx.util.Duration;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.Collections;
import java.util.function.Function;

public class GameController {

    private static Action<NotificationPackage> notificationHandler;
    public Player[] players;

    @FXML
    private AnchorPane catanBoardPane;
    @FXML
    private FlowPane materialsFlow;
    @FXML
    private FlowPane buttonsFlow;
    @FXML
    private VBox playersHBox;
    @FXML
    private Label globalTime;
    @FXML
    private Label turnTime;
    @FXML
    private FlowPane dicePane;
    @FXML
    private Rectangle turnRect;
    @FXML
    private VBox actionsBox;
    @FXML
    private VBox notificationBar;

    private Color[] shuffleColors() {
        var list = Arrays.asList(Color.RED, Color.WHITE, Color.GREEN, Color.ORANGE);
        Collections.shuffle(list);

        Color[] array = new Color[4];
        list.toArray(array);

        return array;
    }

    public void initialize(Window window, byte playerCount, String localeName)
            throws Exception, UnknownHostException, IOException {

        System.out.println("Locale Player name: " + localeName);
        Color[] colors = shuffleColors();

        turnRect.setFill(colors[0]);
        turnRect.setOpacity(0.5);

        SocketClient sock = new SocketClient();
        LocalPlayer local = new LocalPlayer((byte) 0, localeName, colors[0], sock);

        players = new Player[playerCount];
        players[0] = local;
        EnemyPlayer.fillPlayers(players, colors, playerCount);

        DicePane dices = new DicePane(dicePane, new Function<Void, byte[]>() {
            @Override
            public byte[] apply(Void t) {
                try {
                    return sock.rollDice();
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                    restart();
                    return null; // or throw a custom exception if needed
                }

            }
        });

        @SuppressWarnings("unused")
        UserInterface userInterface = new UserInterface(local, materialsFlow, buttonsFlow, dices, actionsBox);

        @SuppressWarnings("unused")
        PlayersFlow playersFlow = new PlayersFlow(players, playersHBox);

        byte[] landsBytes = sock.getLands();
        byte[] harborsBytes = sock.getHarbors();

        System.out.println(Arrays.toString(harborsBytes));

        CatanBoard.Initialize(catanBoardPane, landsBytes, harborsBytes);

        initializeWindowEvents(window);

    }

    private void restart() {
        throw new RuntimeException();
    }

    private void initializeWindowEvents(Window window) {
        Interpolator interpolate = Interpolator.SPLINE(0, 0.6, 0.3, 1);

        final double smoothness = 0.2;

        ChangeListener<? super Number> windowScaleListener = (obs, oldVal, newVal) -> {
            final double calculation = window.getHeight() / 735;
            final double scale = Math.min(1.5, Math.max(calculation, 0.5));
            ScaleTransition scaleTo = new ScaleTransition(Duration.seconds(smoothness), catanBoardPane);
            scaleTo.setToX(scale);
            scaleTo.setToY(scale);
            scaleTo.setInterpolator(interpolate);
            scaleTo.play();
        };

        window.widthProperty().addListener(windowScaleListener);
        window.heightProperty().addListener(windowScaleListener);

        Long startTime = System.currentTimeMillis();
        ObjectProperty<Duration> timer = new SimpleObjectProperty<>(Duration.seconds(0));

        globalTime.textProperty().bind(Bindings.createStringBinding(() -> {
            return String.format("%02d:%01d%.2f",
                    (int) (timer.get().toMinutes()),
                    (int) (timer.get().toSeconds() % 60) / 10,
                    ((timer.get().toSeconds() % 10)));
        }, timer));

        AnimationTimer animationTimer = new AnimationTimer() {
            public void handle(long arg0) {
                timer.setValue(Duration.millis(System.currentTimeMillis() - startTime));
            }
        };

        animationTimer.start();

        GameController.notificationHandler = new Action<NotificationPackage>() {
            @Override
            public void action(NotificationPackage param) {
                notificationBar.getChildren().add(new NotificationGroup(param, (notificationGroup) -> {
                    notificationBar.getChildren().remove(notificationGroup);
                }));
            }
        };
        System.out.println("start!");
    }

    public void showMemoryMonitor(ActionEvent event) {
        // MemoryMonitor memoryMonitor = new MemoryMonitor();
        // memoryMonitor.show();
    }

    public void showStore(ActionEvent event) {
        StoreWindow store = new StoreWindow();
        store.show();
    }

    public void close(ActionEvent event) {
        Platform.exit();
    }

    public static void addNotification(NotificationPackage pack) {
        if (notificationHandler != null)
            notificationHandler.action(pack);
    }

}
