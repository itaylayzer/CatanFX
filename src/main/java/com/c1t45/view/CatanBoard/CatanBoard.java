package com.c1t45.view.CatanBoard;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

import javafx.animation.FadeTransition;
import javafx.animation.Interpolator;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.scene.Cursor;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;

import java.util.Set;
import java.util.TreeSet;

import com.c1t45.view.Constants;
import com.c1t45.view.LocalPlayer;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Interfaces.Condition;
import com.c1t45.view.Interfaces.Predicate;
import com.c1t45.view.Packages.ImagePackage;
import com.c1t45.view.Packages.LinePackage;
import com.c1t45.view.Utils.DoubleUtils;
import com.c1t45.view.Utils.ImageUtils;
import com.c1t45.view.Utils.ShapeUtils;
import com.c1t45.view.Utils.TimeUtils;
import com.c1t45.view.Utils.ValueException;

public class CatanBoard {

    private static CatanBoard instance;

    private final double size = 50, v = Math.sqrt(3) / 2.0;
    private final int[] rows = { 3, 4, 5, 4, 3 };
    private final double xOffset = 88;
    private final double yOffset = 2;

    private List<LandGroup> lands;
    private Group landsGroup;
    // private double scale;

    // vertex
    private Point2D[] vertecies;
    private Circle[] vertexCircles;
    private Group vertexGroup;
    // harbor
    private Group harborsGroup;
    // edges
    private LinePackage[] edges;
    private Rectangle[] edgesRectangles;
    private Group edgesGroup;

    // final groups
    private Runnable lastCancel;
    private Group settlementsGroup;
    private Group roadsGroup;
    // private Group linesGroup;

    private HashMap<Byte, ImageView> settlementsMap;

    public static void clear() {
        instance = null;
    }

    public static CatanBoard Initialize(AnchorPane pane,
            byte[] landsBytes,
            byte[] harborsBytes,
            LocalPlayer player,
            byte[] edgesByte)
            throws ValueException, IndexOutOfBoundsException {
        if (instance != null) {
            throw new ValueException();
        }
        CatanBoard board = new CatanBoard(pane, landsBytes, harborsBytes, edgesByte);
        instance = board;

        TimeUtils.waitUntil((t) -> {
            return instance.edgesGroup.getScene() != null;
        }, null, () -> {
            player.initialize();
        });

        return board;
    }

    Point2D harbordCords(Point2D A, Point2D B, Point2D middle) {
        Point2D harbor = new Point2D(0, 0);

        harbor = harbor.add((A.getX() + B.getX()) / 2, (A.getY() + B.getY()) / 2);

        return harbor.add(harbor.subtract(middle).multiply(0.1));
    }

    private CatanBoard(AnchorPane pane, byte[] landsBytes, byte[] harborsBytes, byte[] edgesByte)
            throws IndexOutOfBoundsException, ValueException {
        lastCancel = null;

        lands = new ArrayList<>();
        landsGroup = new Group();
        pane.setPrefHeight(405);
        pane.setPrefWidth(436);

        DropShadow dropShadow = new DropShadow();
        dropShadow.setOffsetX(0);
        dropShadow.setOffsetY(0);
        dropShadow.setRadius(15);
        dropShadow.setSpread(.8);
        dropShadow.setColor((Color) Paint.valueOf("#1da2d8"));
        pane.setEffect(dropShadow);

        Point2D boardMiddle = initGraph(landsBytes, edgesByte);

        vertexGroup = new Group();
        harborsGroup = new Group();
        edgesGroup = new Group();
        roadsGroup = new Group();

        edgesRectangles = new Rectangle[edges.length];
        vertexCircles = new Circle[vertecies.length];

        settlementsGroup = new Group();
        settlementsMap = new HashMap<>();

        initGraphGraphics(harborsBytes, boardMiddle);

        pane.getChildren().add(harborsGroup);
        pane.getChildren().add(this.landsGroup);

        pane.getChildren().add(edgesGroup);
        pane.getChildren().add(roadsGroup);

        pane.getChildren().add(vertexGroup);
        pane.getChildren().add(settlementsGroup);

        vertexGroup.setOpacity(0);
        vertexGroup.setMouseTransparent(true);

        edgesGroup.setOpacity(0);
        edgesGroup.setMouseTransparent(true);
    }

    private void initGraphGraphics(byte[] harborsBytes, Point2D boardMiddle) {
        for (int vertexOffset = 0; vertexOffset < vertecies.length; vertexOffset++) {
            Point2D vertex = vertecies[vertexOffset];

            Circle circle = new Circle();
            double color = (double) vertexOffset / (double) (vertecies.length - 1);

            circle.setFill(vertexOffset == 0 ? Color.WHITE : new Color(color, color, color, 1.0));
            circle.setRadius(5);

            circle.setLayoutX(vertex.getX());
            circle.setLayoutY(vertex.getY());

            circle.setCursor(Cursor.HAND);

            vertexCircles[vertexOffset] = circle;
            vertexGroup.getChildren().add(circle);
        }

        for (int harborOffset = 0; harborOffset < harborsBytes.length / 2; harborOffset++) {
            byte vertexAOffset = harborsBytes[harborOffset * 2];
            byte vertexBOffset = harborsBytes[harborOffset * 2 + 1];

            Point2D vertexA = vertecies[vertexAOffset];
            Point2D vertexB = vertecies[vertexBOffset];

            Point2D harbPoint = this.harbordCords(vertexA, vertexB, boardMiddle);
            Paint color = harborOffset < Constants.Packages.materials.length
                    ? Constants.Packages.materials[harborOffset].color
                    : Color.WHITE;

            Circle circle = new Circle();

            circle.setFill(color);
            circle.setRadius(5);

            circle.setLayoutX(harbPoint.getX());
            circle.setLayoutY(harbPoint.getY());

            circle.setCursor(Cursor.HAND);

            Line line = new Line(vertexA.getX(), vertexA.getY(), harbPoint.getX(), harbPoint.getY());

            line.setStroke(color);
            line.setStrokeWidth(3);
            harborsGroup.getChildren().add(line);

            line = new Line(vertexB.getX(), vertexB.getY(), harbPoint.getX(), harbPoint.getY());

            line.setStroke(color);
            line.setStrokeWidth(3);

            harborsGroup.getChildren().add(line);
            harborsGroup.getChildren().add(circle);
        }

        for (int edgeOffset = 0; edgeOffset < edges.length; edgeOffset++) {
            LinePackage edge = edges[edgeOffset];

            Rectangle rect = new Rectangle();
            double color = (double) edgeOffset / (double) (edges.length - 1);

            rect.setFill(edgeOffset == 0 ? Color.WHITE : new Color(color, color, color, 1.0));

            final double size = 10;

            rect.setHeight(size);
            rect.setWidth(size);

            rect.setArcHeight(size / 2);
            rect.setArcWidth(size / 2);

            rect.setLayoutX(edge.middle.getX() - size / 2);
            rect.setLayoutY(edge.middle.getY() - size / 2);

            rect.setCursor(Cursor.HAND);

            edgesRectangles[edgeOffset] = rect;
            edgesGroup.getChildren().add(rect);
        }

    }

    private Point2D initGraph(byte[] landsByte, byte[] edgesByte) {

        Comparator<Point2D> doubleComparator = new Comparator<Point2D>() {
            @Override
            public int compare(Point2D p1, Point2D p2) {
                int x1 = DoubleUtils.roundToTwoDecimals(p1.getX());
                int x2 = DoubleUtils.roundToTwoDecimals(p2.getX());
                int y1 = DoubleUtils.roundToTwoDecimals(p1.getY());
                int y2 = DoubleUtils.roundToTwoDecimals(p2.getY());
                int compareX = Integer.compare(x1, x2);
                int compareY = Integer.compare(y1, y2);
                return compareY == 0 ? compareX : compareY;
            }
        };

        Comparator<LinePackage> edgeComparator = new Comparator<LinePackage>() {
            @Override
            public int compare(LinePackage p1, LinePackage p2) {
                return doubleComparator.compare(p1.middle, p2.middle);
            }
        };

        Point2D middle = initVerteciesAndHexagons(landsByte, doubleComparator);
        initEdges(edgesByte, edgeComparator);
        return middle;
    }

    private Point2D initVerteciesAndHexagons(byte[] landsBytes, Comparator<Point2D> doubleComparator) {

        int resourceIndex = 0;

        Set<Point2D> verticesSet = new TreeSet<>(doubleComparator);
        Point2D boardMiddle = new Point2D(0, 0);
        for (int row = 0; row < rows.length; row++) {
            int cols = rows[row];
            for (int col = 0; col < cols; col++) {
                double x = xOffset + col * size * v * 2;
                double y = yOffset + row * size * 1.5;

                x -= size * (row <= 2 ? row : 4 - row) * 0.75 / v;

                ImagePackage info = null;
                Byte landNum = 7;

                double[] hexagonPoints = ShapeUtils.hexagon(x, y, size, v);
                int mX = 0, mY = 0;

                for (int i = 0; i < hexagonPoints.length; i += 2) {
                    double vx = hexagonPoints[i];
                    mX += vx;
                    double vy = hexagonPoints[i + 1];
                    mY += vy;
                    Point2D vertex = new Point2D(vx, vy);
                    verticesSet.add(vertex);

                    // int j = i < 10 ? i : i - 12;
                    // nx = hexagonPoints[j + 2];
                    // ny = hexagonPoints[j + 3];

                    // LinePackage line = new LinePackage(vx, vy, nx, ny);

                    // edgesSet.add(line);

                }
                if (row == 2 && col == 2) {
                    boardMiddle = new Point2D(mX / 6, mY / 6);
                }
                byte landVal = landsBytes[resourceIndex++];
                landNum = (Byte) (byte) (landVal >> 3);
                if (landNum != 7)
                    info = Constants.Packages.materials[landVal & 0x07];

                LandGroup land = new LandGroup(info, landNum, x, y, size, v);
                landsGroup.getChildren().add(land);

                this.lands.add(land);

            }
        }

        vertecies = new Point2D[verticesSet.size()];
        verticesSet.toArray(vertecies);
        return boardMiddle;
    }

    private void initEdges(byte[] edgesByte, Comparator<LinePackage> edgeComparator) {
        final byte AREAS = 19;
        Set<LinePackage> edgesSet = new TreeSet<>(edgeComparator);

        int offset = 0;

        // pass the lands to vertecies x area
        for (; offset < edgesByte.length && Math.min(edgesByte[offset], edgesByte[offset + 1]) < AREAS; offset += 2)
            ;

        // continue to vertecies x vertecies area
        for (; offset < edgesByte.length; offset += 2) {
            byte from = (byte) (edgesByte[offset] - AREAS),
                    to = (byte) (edgesByte[offset + 1] - AREAS);

            Point2D start = vertecies[from];
            Point2D end = vertecies[to];

            edgesSet.add(new LinePackage(start.getX(), start.getY(), end.getX(), end.getY(), from, to));

        }

        offset = 0;
        for (LinePackage line : edgesSet) {
            line.offset = (byte) (offset++);
        }
        edges = new LinePackage[edgesSet.size()];
        edgesSet.toArray(edges);
        edgesSet = null;

    }

    public static void addHouse(byte vertex, Color color) {
        instance.addSettlement(Constants.Images.house, vertex, color);
    }

    public static void addCity(byte vertex, Color color) {
        instance.addSettlement(Constants.Images.city, vertex, color);
    }

    public static void addRoad(byte edge, Color color) {
        instance.addLine(edge, color);
    }

    private void addSettlement(Image image, byte index, Color color) {
        final double size = 15;
        boolean isCity = settlementsMap.containsKey(index);
        Point2D vertex = vertecies[index];

        ImageView view = new ImageView(ImageUtils.colorChange(image, color));
        Circle circle = vertexCircles[index];

        if (isCity) {
            view = settlementsMap.get(index);
            view.setImage(ImageUtils.colorChange(image, color));
            final double scale = 1.3;
            view.setScaleX(scale);
            view.setScaleY(scale);
            circle.setCursor(Cursor.DEFAULT);
            circle.setScaleX(scale);
            circle.setScaleY(scale);
        }

        view.setMouseTransparent(true);
        view.setFitHeight(size);
        view.setFitWidth(size);
        view.setLayoutX(vertex.getX() - size / 2);
        view.setLayoutY(vertex.getY() - size / 2);
        view.setPickOnBounds(false);

        circle.setRadius(15);
        circle.setLayoutX(vertex.getX());
        circle.setLayoutY(vertex.getY());
        circle.setOpacity(1);

        circle.setFill(Paint.valueOf("#1a1a1a"));

        if (!isCity) {
            settlementsGroup.getChildren().add(circle);
            settlementsGroup.getChildren().add(view);
            settlementsMap.put(index, view);
        }

        view.toFront();

    }

    private void addLine(byte lineNum, Color color) {
        double sX = this.edges[lineNum].start.getX(),
                sY = this.edges[lineNum].start.getY(),
                eX = this.edges[lineNum].end.getX(),
                eY = this.edges[lineNum].end.getY();

        // System.out.println("offsets-diffrent=" + (this.edges[lineNum].offset -
        // lineNum));
        Line line = new Line(sX, sY, eX, eY);

        line.setStrokeWidth(4);
        line.setStroke(color);

        roadsGroup.getChildren().add(line);
    }

    public static CatanBoard getInstance() {
        return instance;
    }

    public FadeTransition fade(Group group, double to) {
        FadeTransition transition = new FadeTransition(Duration.seconds(0.2), group);
        transition.setToValue(to);
        transition.setInterpolator(Interpolator.SPLINE(0, 0.3, 0, 1));
        transition.play();
        return transition;
    }

    public void pickVertex(Condition<Byte> allowVertex, Runnable onCancel, Action<Byte> onPicked) {

        Scene scene = vertexGroup.getScene();
        lastCancel = onCancel;
        EventHandler<KeyEvent> cancelEvent = new EventHandler<>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                if (keyEvent.getCode() == KeyCode.ESCAPE) {
                    scene.removeEventHandler(KeyEvent.KEY_PRESSED, this);
                    cancelCurrentPick(true);
                }
            }
        };
        scene.addEventHandler(KeyEvent.KEY_PRESSED, cancelEvent);

        applyPickVertexUI(allowVertex, new Action<Byte>() {
            @Override
            public void action(Byte param) {
                onPicked.action(param);
                cancelPickVertexUI(true);
            }
        });

    }

    public void pickEdge(Condition<Byte> allowVertex, Runnable onCancel, Action<Byte> onPicked) {

        Scene scene = edgesGroup.getScene();
        lastCancel = onCancel;
        EventHandler<KeyEvent> cancelEvent = new EventHandler<>() {
            @Override
            public void handle(KeyEvent keyEvent) {
                if (keyEvent.getCode() == KeyCode.ESCAPE) {

                    scene.removeEventHandler(KeyEvent.KEY_PRESSED, this);
                    cancelCurrentPick(true);

                }
            }
        };
        scene.addEventHandler(KeyEvent.KEY_PRESSED, cancelEvent);

        applyPickEdgeUI(allowVertex, new Action<Byte>() {
            @Override
            public void action(Byte param) {
                onPicked.action(param);
                cancelPickEdgeUI(true);
            }
        });

    }

    private void applyPickVertexUI(Condition<Byte> allowVertex, Action<Byte> action) {
        vertexGroup.setMouseTransparent(false);
        settlementsGroup.setMouseTransparent(false);

        fade(landsGroup, 0.5);
        fade(vertexGroup, 1);
        int index;
        for (index = 0; index < vertexCircles.length; index++) {
            Circle circle = vertexCircles[index];
            if (allowVertex.condition((byte) index)) {
                final byte pickedVertex = (byte) index;
                circle.setOpacity(1);
                circle.setOnMouseClicked((event) -> {
                    action.action(pickedVertex);
                });
                circle.onMouseClickedProperty().addListener((e, a, b) -> {

                });
            } else {
                if (!settlementsGroup.getChildren().contains(circle))
                    circle.setOpacity(0);
                circle.setOnMouseClicked(null);
            }
        }
    }

    private void cancelPickVertexUI(boolean useFade) {
        vertexGroup.setMouseTransparent(true);
        settlementsGroup.setMouseTransparent(true);

        if (useFade) {
            fade(landsGroup, 1);
            fade(vertexGroup, 0);
        } else {
            landsGroup.setOpacity(1);
            vertexGroup.setOpacity(0);
        }
        for (Circle circle : vertexCircles) {
            circle.setOnMouseClicked(null);
        }

    }

    private void applyPickEdgeUI(Condition<Byte> allowVertex, Action<Byte> action) {
        edgesGroup.setMouseTransparent(false);
        roadsGroup.setMouseTransparent(false);

        fade(landsGroup, 0.5);
        fade(edgesGroup, 1);
        int index;
        for (index = 0; index < edgesRectangles.length; index++) {
            Rectangle rect = edgesRectangles[index];
            Byte byteIndex = (byte) index;
            if (allowVertex.condition(byteIndex)) {
                final byte pickedVertex = (byte) index;
                rect.setOpacity(1);
                rect.setOnMouseClicked((event) -> {
                    action.action(pickedVertex);
                });
            } else {
                if (!roadsGroup.getChildren().contains(rect))
                    rect.setOpacity(0);
                rect.setOnMouseClicked(null);
            }
        }
    }

    private void cancelPickEdgeUI(boolean useFade) {
        edgesGroup.setMouseTransparent(true);
        roadsGroup.setMouseTransparent(true);

        if (useFade) {
            fade(landsGroup, 1);
            fade(edgesGroup, 0);
        } else {
            landsGroup.setOpacity(1);
            edgesGroup.setOpacity(0);
        }
        for (Rectangle rect : edgesRectangles) {
            rect.setOnMouseClicked(null);
        }

    }

    public void cancelCurrentPick(boolean useFade) {
        if (lastCancel != null)
            lastCancel.run();
        lastCancel = null;
        cancelPickVertexUI(useFade);
        cancelPickEdgeUI(useFade);
    }

    public byte[] seperateEdge(Byte picked) {
        LinePackage line = edges[picked];
        return new byte[] { line.from, line.to };
    }
}
