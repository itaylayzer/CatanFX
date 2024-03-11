package com.c1t45.controller;

public interface Observer<T> {
    void update(T previous,T current);
}
