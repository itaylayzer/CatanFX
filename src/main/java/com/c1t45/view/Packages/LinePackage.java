package com.c1t45.view.Packages;

import javafx.geometry.Point2D;

public class LinePackage {
    final public Point2D start;
    final public Point2D end;
    final public Point2D middle;
    public byte offset;

    public LinePackage(double startX, double startY, double endX, double endY) {
        this.middle = new Point2D((startX + endX) / 2, (startY + endY) / 2);
        this.start = new Point2D(startX, startY);
        this.end = new Point2D(endX, endY);
    }

}
