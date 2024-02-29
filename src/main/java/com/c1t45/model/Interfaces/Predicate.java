package com.c1t45.model.Interfaces;

public interface Predicate<T> {
    boolean condition(T value, int index);
}
