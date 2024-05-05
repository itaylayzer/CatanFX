package com.c1t45.view.Packages;

import java.util.ArrayList;
import java.util.List;

import javafx.geometry.Point2D;

public class VertexPackage {
    public byte index;
    public List<LinePackage> lines;
    public Point2D point;

    public VertexPackage(Point2D _point) {
        lines = new ArrayList<>();
        point = _point;
    }
}
