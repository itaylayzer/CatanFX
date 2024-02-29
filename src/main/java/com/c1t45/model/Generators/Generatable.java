package com.c1t45.model.Generators;

public interface Generatable<T extends Object> {
    T generate() throws IndexOutOfBoundsException;
}
