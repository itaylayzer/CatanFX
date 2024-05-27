package com.c1t45.view.Interfaces;

public interface Generatable<T extends Object> {
    T generate() throws IndexOutOfBoundsException;
}
