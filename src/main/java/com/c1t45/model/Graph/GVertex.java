package com.c1t45.model.Graph;

import java.util.Comparator;
import java.util.TreeSet;

public class GVertex {
    public short index;
    public GInfo info;
    public TreeSet<GEdge> edges;
    private static final Comparator<GEdge> comperator;

    static {
        comperator = new Comparator<GEdge>() {
            @Override
            public int compare(GEdge o1, GEdge o2) {
                return o1.getIndex() - o2.getIndex();
            }
        };
    }

    public GVertex(short index, GInfo info) {
        this.index = index;
        this.info = info;
        this.edges = new TreeSet<>(comperator);
    }
}