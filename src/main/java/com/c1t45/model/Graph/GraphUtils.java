package com.c1t45.model.Graph;

import java.util.ArrayDeque;
import java.util.Deque;

import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;

public class GraphUtils {
    public int findLongestRoadWithSameWeight(short weight, Graph graph) {
        ObjectProperty<Integer> longestRoadCount = new SimpleObjectProperty<>(0);

        graph.forEach((vertex) -> {
            vertex.info.visited = false;
        });

        graph.forEach((vertex) -> {
            if (!vertex.info.visited) {
                longestRoadCount.setValue(Math.max(DFS(vertex, weight), longestRoadCount.getValue()));
            }
        });

        return longestRoadCount.getValue();
    }

    public static int DFS(GVertex start, short weight) {
        Deque<GVertex> stack = new ArrayDeque<>();

        stack.push(start);
        start.info.visited = true;
        start.info.longestRoadCount = 1;
        start.info.longestRoadWeight = start.info.maxWeight = 0;

        int longestRoadCount = 0;

        while (!stack.isEmpty()) {
            GVertex current = stack.pop();

            for (GEdge edge : current.edges) {
                GVertex neighbor = edge.vertex;
                if (!neighbor.info.visited && edge.weight == weight) {
                    neighbor.info.visited = true;

                    neighbor.info.longestRoadCount = (short) (current.info.longestRoadCount + 1);
                    neighbor.info.longestRoadWeight = (short) (current.info.longestRoadWeight
                            + edge.weight);
                    neighbor.info.maxWeight = (short) Math.max(neighbor.info.maxWeight,
                            neighbor.info.longestRoadWeight);

                    stack.push(neighbor);

                    if (neighbor.info.longestRoadCount > longestRoadCount) {
                        longestRoadCount = neighbor.info.longestRoadCount;
                    }
                }
            }
        }

        return longestRoadCount;
    }
}
