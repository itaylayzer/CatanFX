package com.itayl.view;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import com.itayl.view.Interfaces.Cancelable;

public class Timeout {

    public static Cancelable timeout(Runnable task, long timeoutMs) {

        ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();

        executor.schedule(task, timeoutMs, TimeUnit.MILLISECONDS);
        executor.shutdown();

        return new Cancelable() {
            @Override
            public void cancel() {

                executor.shutdownNow();
            }
        };
    }
}
