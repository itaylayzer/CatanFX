package com.c1t45.view.Interfaces;

public interface Predicate<T> {
    boolean condition(T value, int index);
}
