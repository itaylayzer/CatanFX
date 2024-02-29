package com.c1t45.model.Graph;

public class GEdge {
    public GVertex vertex;
    public short weight;

    public GEdge(GVertex vertex, short weight) {
        this.vertex = vertex;
        this.weight = weight;
    }

    public short getIndex() {
        return this.vertex.index;
    }
}