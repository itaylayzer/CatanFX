package com.itayl.view.Interfaces;

public interface Generatable<T extends Object> {
    T generate() throws IndexOutOfBoundsException;
}
