package com.itayl.controller;

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

import com.itayl.view.EnemyPlayer;
import com.itayl.view.LocalPlayer;
import com.itayl.view.Player;
import com.itayl.view.UserInterface;
import com.itayl.view.Components.CatanBoard.CatanBoard;
import com.itayl.view.Components.CatanBoard.DicePane;
import com.itayl.view.Components.CatanBoard.NotificationGroup;
import com.itayl.view.Components.Navbar.PlayersFlow;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Packages.NotificationPackage;
import com.itayl.view.Utils.BytesUtils;
import com.itayl.view.Windows.StoreWindow;

public class GameController {

    public static Action<NotificationPackage> notificationHandler;
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
                    System.out.println("rolls" + BytesUtils.bytesString(rolls));
                    local.update();

                    if (rolls[0] + rolls[1] == 7) {

                        CatanBoard board = CatanBoard.getInstance();
                        // board.cancelCurrentPick(false);
                        board.setInterfaceDisabled(true);
                        byte counts = local.getMaterialsCount();
                        byte[] mats = local.getMaterials();
                        if (counts >= 7) {
                            try {
                                byte amountToDrop = (byte) (counts / 2);

                                board.materialCounts((a) -> {
                                    sock.removeMats(a);
                                    board.pickHexagon((value) -> {
                                        return value != 9 && board.getRobberPos() != value;
                                    }, () -> {
                                    }, (picked) -> {
                                        board.setRobberPos(picked);

                                        try {
                                            byte allowed = sock.nearbyPlayers(picked);
                                            System.out.println("allowed  " + BytesUtils.singleString(allowed));
                                            if ((allowed & 0x0F) != 0) {
                                                board.playerSelect(allowed, (player) -> {
                                                    local.moveRobber(picked, player);
                                                    local.update();

                                                }, false);
                                            } else {
                                                local.moveRobber(picked, (byte) 4);
                                                local.update();
                                            }
                                        } catch (Exception exp) {
                                            exp.printStackTrace(System.err);
                                        }

                                    });

                                }, amountToDrop, mats);
                            } catch (Exception exp) {
                                exp.printStackTrace(System.err);
                            }
                        } else {
                            board.pickHexagon((value) -> {
                                return value != 9 && board.getRobberPos() != value;
                            }, () -> {
                            }, (picked) -> {
                                board.setRobberPos(picked);

                                try {
                                    byte allowed = sock.nearbyPlayers(picked);
                                    if (allowed != 0) {
                                        board.playerSelect(allowed, (player) -> {
                                            board.setInterfaceDisabled(false);
                                            local.moveRobber(picked, player);
                                            local.update();

                                        }, false);
                                    } else {
                                        board.setInterfaceDisabled(false);
                                        local.moveRobber(picked, (byte) 4);
                                        local.update();
                                    }
                                } catch (Exception exp) {
                                    exp.printStackTrace(System.err);
                                }

                            });
                        }

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
                edges, dices);

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
