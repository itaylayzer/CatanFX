package com.c1t45.view.CatanBoard;

import javafx.animation.FadeTransition;
import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.ScaleTransition;
import javafx.animation.Timeline;
import javafx.scene.image.ImageView;
import javafx.scene.layout.FlowPane;
import javafx.util.Duration;

import java.util.Random;
import java.util.function.Function;

import com.c1t45.view.Constants;
import com.c1t45.view.Interfaces.Action2;

public class DicePane {
    private ImageView dice1;
    private ImageView dice2;
    private Random random;
    private Function<Void, byte[]> socketDice;
    private static final double smoothness = Math.sqrt(2);
    private static final double end = 0.5;

    public DicePane(FlowPane father, Function<Void, byte[]> socketDice) {
        dice1 = new ImageView(Constants.Images.dices[0]);
        dice2 = new ImageView(Constants.Images.dices[1]);

        final double size = 80;

        dice1.setFitHeight(size);
        dice1.setFitWidth(size);
        dice2.setFitHeight(size);
        dice2.setFitWidth(size);

        this.random = new Random();
        this.socketDice = socketDice;

        father.getChildren().addAll(dice1, dice2);
    }

    private void rollRec(Function<Double, byte[]> rollFunction,
            Action2<Byte, Byte> onFinish,
            double current) {
        if (current >= end) {
            if (onFinish != null) {
                byte[] roll_result = this.socketDice.apply(null);
                onFinish.action(roll_result[0], roll_result[1]);
            }
        } else {
            Timeline timeline = new Timeline();
            timeline.getKeyFrames().add(new KeyFrame(Duration.seconds(current), (e) -> {
                rollFunction.apply(current);
                rollRec(rollFunction, onFinish, current * smoothness);
            }));
            timeline.play();
        }
    }

    public void randRoll(Action2<Byte, Byte> onFinish) {
        this.rollRec(new Function<Double, byte[]>() {
            @Override
            public byte[] apply(Double current) {
                byte c1 = (byte) random.nextInt(1, 7);
                byte c2 = (byte) random.nextInt(1, 7);

                double from = current * smoothness >= end ? 1.1 : 0.7;
                double to = current * smoothness >= end ? 1 : 0.9;

                scaleFromTo(dice1, from, to, current);
                scaleFromTo(dice2, from, to, current);

                dice1.setImage(Constants.Images.dices[c1 - 1]);
                dice2.setImage(Constants.Images.dices[c2 - 1]);

                return new byte[] { c1, c2 };
            }

            private void scaleFromTo(ImageView dice, double from, double to, double time) {

                Duration deltaTime = Duration.seconds(time);

                ScaleTransition scaleTransition = new ScaleTransition(deltaTime, dice);
                scaleTransition.setFromX(from);
                scaleTransition.setFromY(from);
                scaleTransition.setToX(to);
                scaleTransition.setToY(to);
                scaleTransition.setInterpolator(Interpolator.SPLINE(.3, 0, 0, 1));

                FadeTransition fadeTransition = new FadeTransition(deltaTime, dice);
                fadeTransition.setFromValue(0.7);
                fadeTransition.setToValue(1);
                fadeTransition.setInterpolator(Interpolator.SPLINE(.3, 0, 0, 1));

                scaleTransition.play();
                if (from < to)
                    fadeTransition.play();
            }
        }, onFinish, 0.01);
    }
}
