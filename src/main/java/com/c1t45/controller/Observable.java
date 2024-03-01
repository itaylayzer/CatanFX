package com.c1t45.controller;

import java.util.ArrayList;
import java.util.List;

public abstract class Observable {
    private List<Observer> observers;

    public Observable(){
        observers = new ArrayList<>();
    }

    protected void notifyObservers(){
        for (Observer observer: observers){
            observer.update();
        }
    }

    public void addObserver(Observer observer){
        observers.add(observer);
    }
}
