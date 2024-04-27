package com.c1t45.view.Generators;

public interface Generatable<T extends Object> {
    T generate() throws IndexOutOfBoundsException;
}
