package com.c1t45.model.Graph;

import com.c1t45.model.Interfaces.Action;

public class Graph {
    private GVertex[] vertecies;
    private int size;

    public Graph(int size) {
        this.size = size;
        this.vertecies = new GVertex[size];
    }

    public void joinWt(short A, short B, short weight) throws ArrayIndexOutOfBoundsException {
        vertecies[A].edges.add(new GEdge(vertecies[B], weight));
    }

    public int getSize() {
        return this.size;
    }

    public void remove(short A, short B) {
        vertecies[A].edges.removeIf((edge) -> {
            return edge.getIndex() == B;
        });
    }

    public void setVertex(short index, GInfo vertexInfo) throws ArrayIndexOutOfBoundsException {
        this.vertecies[index] = new GVertex(index, vertexInfo);
    }

    public GVertex getVertex(short index) throws ArrayIndexOutOfBoundsException {
        return this.vertecies[index];
    }

    public Object adjacent(short A, short B) throws ArrayIndexOutOfBoundsException {
        return vertecies[A].edges.stream().filter((edge) -> {
            return edge.getIndex() == B;
        }).toArray().length == 1;
    }

    public void forEach(Action<GVertex> vertexAction) {
        for (GVertex vertex : vertecies) {
            vertexAction.action(vertex);
        }
    }

}
