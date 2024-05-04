package com.c1t45.view;

import java.util.ArrayList;
import java.util.List;

import com.c1t45.view.Interfaces.Action2;
import com.c1t45.view.Interfaces.Predicate;
import com.c1t45.view.Utils.BytesUtils;
import com.c1t45.view.Constants.Materials;

import javafx.scene.paint.Color;

public class Player {

    protected byte id;
    protected String name;
    protected byte[] materials;
    private Color color;
    private static byte turnID;

    private List<Object> devCards;
    private static byte houses[];
    private static byte cities[];
    private static byte roads[];

    @SuppressWarnings("unused")
    private Action2<Byte, Player> onDevcardChange;
    private List<Runnable> onInventoryChange;

    static {
        houses = new byte[Constants.VERTECIES];
        cities = new byte[Constants.VERTECIES];
        roads = new byte[Constants.LINES];

    }

    public static byte[] getHouses() {
        return houses;
    }

    public static byte[] getCities() {
        return cities;
    }

    public Player(byte id, String localeName, Color color) {
        this.id = (byte) (id + 1);
        this.name = localeName;
        this.color = color;

        this.materials = new byte[5];
        this.devCards = new ArrayList<>();
        this.onInventoryChange = new ArrayList<>();
        this.onDevcardChange = null;
    }

    public String getName() {
        return name;
    }

    public byte getMaterialsCount() {
        byte materialsCount = 0;

        for (byte mat : materials) {
            materialsCount += mat;
        }

        return materialsCount;
    }

    public byte getMaxRoadLength() {
        return 0;
    }

    public byte getVicotryPoints() {
        byte index;
        byte count = 0;

        for (index = 0; index < Constants.VERTECIES; index++) {
            if (cities[index] == this.id) {
                count += 2;
            }

            else if (houses[index] == this.id) {
                count++;
            }
        }

        count += devCards.stream().filter((devcard) -> {
            // replace to VictoryPointCart
            return devcard instanceof Runnable;
        }).toArray().length;

        return count;
    }

    public byte getLongestRoadLength() {
        return 0;
    }

    public byte getDevCardsCount() {
        return (byte) devCards.size();
    }

    public Color getColor() {
        return color;
    }

    public byte[] getMaterials() {
        return this.materials;
    }

    protected void callOnInventoryChange() {

        for (Runnable event : onInventoryChange) {
            event.run();
        }
    }

    public void addMaterial(Materials material, byte count) {
        this.materials[material.getValue()] += count;
        callOnInventoryChange();
    }

    protected void addMaterials(byte[] addedMaterials) {
        BytesUtils.add(this.materials, addedMaterials);
    }

    public void addOnInventoryChange(Runnable handler) {
        onInventoryChange.add(handler);
    }

    public void removeOnInventoryChange(Runnable handler) {
        onInventoryChange.remove(handler);
    }

    public boolean enoughMaterials(byte[] store) {
        Predicate<Byte> predicate = new Predicate<Byte>() {
            @Override
            public boolean condition(Byte value, int index) {
                return value < 0;
            }
        };

        byte[] dup = BytesUtils.duplicate(this.materials);
        List<Byte> bytes = BytesUtils.filter(BytesUtils.minus(dup, store), predicate);

        return bytes.size() == 0;
    }

    public static boolean houseDontBelong(Byte value) {
        return houses[value] == 0;
    }

    public static boolean hasHouse(byte id, Byte value) {
        return houses[value] == id + 1;
    }

    public static boolean hasCity(byte id, Byte value) {
        return cities[value] == id + 1;
    }

    public static boolean roadDontBelong(Byte value) {
        return roads[value] == 0;
    }

    public boolean myTurn() {
        return Player.turnID + 1 == this.id;
    }

    static public void setTurnID(byte id) {
        turnID = id;
    }

    protected static void moveTurn() {
        turnID++;
        if (turnID > 2)
            turnID = 0;
    }
}