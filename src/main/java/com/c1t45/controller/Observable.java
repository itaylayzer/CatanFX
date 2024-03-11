package com.c1t45.controller;

import java.util.ArrayList;
import java.util.List;

public abstract class Observable<T> {
    private List<Observer<T>> observers;

    public Observable(){
        observers = new ArrayList<>();
    }

    protected final void notifyObservers(T previous, T current){
        for (Observer<T> observer: observers){
            observer.update(previous, current);
        }
    }

    public final void addObserver(Observer<T> observer){
        observers.add(observer);
    }
}
