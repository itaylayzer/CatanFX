package com.itayl.view.Interfaces;

public interface Predicate<T> {
    boolean condition(T value, int index);
}
