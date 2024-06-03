package com.itayl.view.Utils;

import com.itayl.view.Interfaces.Condition;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.util.Duration;

public class TimeUtils {
    public static <T> void waitUntil(Condition<T> condition, T param, Runnable action) {
        final Timeline[] timeline = { null }; // Using final array to hold Timeline

        timeline[0] = new Timeline(new KeyFrame(Duration.seconds(0.1), event -> {
            if (condition.condition(param)) {
                action.run();
                if (timeline[0] != null) {
                    timeline[0].stop();
                }
            }
        }));
        timeline[0].setCycleCount(Timeline.INDEFINITE);
        timeline[0].play();
    }

}
