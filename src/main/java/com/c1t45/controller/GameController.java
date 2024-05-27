package com.c1t45.controller;

import com.c1t45.view.EnemyPlayer;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Player;
import com.c1t45.view.UserInterface;
import com.c1t45.view.Components.CatanBoard.CatanBoard;
import com.c1t45.view.Components.CatanBoard.DicePane;
import com.c1t45.view.Components.CatanBoard.NotificationGroup;
import com.c1t45.view.Components.Navbar.PlayersFlow;
import com.c1t45.view.Interfaces.Action;
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
import javafx.scene.layout.HBox;
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
    private AnimationTimer turnTimer;

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
    @FXML
    private HBox inventoryBox;
    @FXML
    private VBox selectionBox;

    private Color[] shuffleColors() {
        var list = Arrays.asList(Color.RED, Color.WHITE, Color.GREEN, Color.ORANGE);
        Collections.shuffle(list);

        Color[] array = new Color[4];
        list.toArray(array);

        return array;
    }

    public void initialize(Window window, byte playerCount, String localeName, byte[] edges)
            throws Exception, UnknownHostException, IOException {

        Color[] colors = shuffleColors();

        SocketClient sock = new SocketClient();
        LocalPlayer local = new LocalPlayer((byte) 0, localeName, colors[0], sock);

        Player[] players = new Player[playerCount];
        players[0] = local;
        EnemyPlayer.fillPlayers(players, colors, playerCount);

        DicePane dices = new DicePane(dicePane, new Function<Void, byte[]>() {
            @Override
            public byte[] apply(Void t) {
                try {
                    byte[] rolls = sock.rollDice();
                    local.update();

                    if (rolls[0] + rolls[1] == 7) {
                        CatanBoard board = CatanBoard.getInstance();
                        board.cancelCurrentPick(false);

                        board.pickHexagon((value) -> {
                            return value != 9 && board.getRobberPos() != value;
                        }, () -> {
                        }, (picked) -> {
                            board.setRobberPos(picked);

                            // TODO: get from the server available players!!
                            board.playerSelect((player) -> {
                                // TODO: available players!!
                                local.moveRobber(picked, player);
                                byte counts = local.getMaterialsCount();
                                byte[] mats = local.getMaterials();
                                if (counts >= 7) {
                                    byte amountToDrop = (byte) (counts / 2);

                                    board.materialCounts((a) -> {
                                        // byte index;
                                        for (int index = 0; index < a.length; index++) {
                                            mats[index] -= a[index];
                                        }
                                        // TODO: need to update server!
                                        local.callOnInventoryChange();
                                    }, amountToDrop, mats);
                                }
                            }, false);

                        });

                    }

                    return rolls;
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                    restart();
                    return null; // or throw a custom exception if needed
                }

            }
        });

        UserInterface userInterface = new UserInterface(local, materialsFlow, buttonsFlow, inventoryBox, dices,
                actionsBox);

        new PlayersFlow(players, playersHBox);

        byte[] landsBytes = sock.getLands();
        byte[] harborsBytes = sock.getHarbors();

        CatanBoard.Initialize(catanBoardPane,
                userInterface,
                selectionBox,
                colors,
                players,
                landsBytes,
                harborsBytes,
                local,
                edges);

        initializeWindowEvents(window);
        initTurnRect(colors);

    }

    private void initTurnRect(Color[] colors) {
        turnRect.setOpacity(0.5);
        turnTimer = null;

        Action<Byte> event = new Action<>() {
            @Override
            public void action(Byte param) {

                Platform.runLater(() -> {
                    if (turnTimer != null)
                        turnTimer.stop();

                    try {
                        turnRect.setFill(colors[param]);

                        Long startTime = System.currentTimeMillis();
                        ObjectProperty<Duration> timer = new SimpleObjectProperty<>(Duration.seconds(0));

                        turnTime.textProperty().bind(Bindings.createStringBinding(() -> {
                            return String.format("%02d:%01d%.2f",
                                    (int) (timer.get().toMinutes()),
                                    (int) (timer.get().toSeconds() % 60) / 10,
                                    ((timer.get().toSeconds() % 10)));
                        }, timer));

                        turnTimer = new AnimationTimer() {
                            public void handle(long arg0) {
                                timer.setValue(Duration.millis(System.currentTimeMillis() - startTime));
                            }
                        };

                        turnTimer.start();
                    } catch (Exception exp) {

                    }
                });

            }
        };
        event.action((byte) 0);
        Player.addOnTurnIDChange(event);
    }

    private void restart() {
        throw new RuntimeException();
    }

    private void initializeWindowEvents(Window window) {
        Interpolator interpolate = Interpolator.SPLINE(0, 0.6, 0.3, 1);

        final double smoothness = 0.2;

        ChangeListener<? super Number> windowScaleListener = (obs, oldVal, newVal) -> {
            final double calculation = window.getHeight() / 700;
            final double scale = Math.min(1.5, Math.max(calculation, 0.4));
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
