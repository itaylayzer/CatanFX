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

import com.c1t45.model.Interfaces.Action2;
import com.c1t45.view.Constants;

public class DicePane {
    public ImageView dice1;
    public ImageView dice2;
    public Random random;
    private static final double smoothness = Math.sqrt(2);
    private static final double end = 0.5;

    public DicePane(FlowPane father) {
        dice1 = new ImageView(Constants.Images.dices[0]);
        dice2 = new ImageView(Constants.Images.dices[1]);

        final double size = 80;

        dice1.setFitHeight(size);
        dice1.setFitWidth(size);
        dice2.setFitHeight(size);
        dice2.setFitWidth(size);

        random = new Random();

        father.getChildren().addAll(dice1, dice2);
    }

    private void rollRec(Function<Double, byte[]> rollFunction,
            Action2<Byte, Byte> onFinish,
            double current,
            byte[] lastValue) {
        if (current >= end) {
            if (onFinish != null)
                onFinish.action(lastValue[0], lastValue[1]);
            return;
        } else {
            Timeline timeline = new Timeline();
            timeline.getKeyFrames().add(new KeyFrame(Duration.seconds(current), (e) -> {
                byte[] value = rollFunction.apply(current);
                rollRec(rollFunction, onFinish, current * smoothness, value);
            }));
            timeline.play();
        }
    }

    public void roll(Action2<Byte, Byte> onFinish) {
        this.rollRec(new Function<Double, byte[]>() {
            @Override
            public byte[] apply(Double current) {
                byte c1 = 5;// (byte) random.nextInt(1, 7);
                byte c2 = 6;// (byte) random.nextInt(1, 7);

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
        }, onFinish, 0.01, new byte[] { 0, 0 });
    }
}
