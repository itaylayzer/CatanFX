package com.c1t45.model.Graph;

public class GInfo {
    public boolean visited;
    public short longestRoadCount;
    public short longestRoadWeight;
    public short maxWeight;

    public GInfo() {
        this.visited = false;
        this.longestRoadCount = 0;
        this.longestRoadWeight = 0;
        this.maxWeight = 0;
    }

}
