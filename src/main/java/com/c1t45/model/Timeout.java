package com.c1t45.model;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import com.c1t45.model.Interfaces.Cancelable;

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
