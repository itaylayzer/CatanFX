package com.c1t45.view;

import java.util.ArrayList;
import java.util.List;

import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Interfaces.Action2;
import com.c1t45.view.Interfaces.Predicate;
import com.c1t45.view.Utils.BytesUtils;
import com.c1t45.view.Constants.Materials;

import javafx.scene.paint.Color;
import java.util.Arrays;

public class Player {
    protected byte id;
    protected String name;
    protected byte[] materials;
    private Color color;
    private static byte turnID;

    private byte[] devcards;
    private static byte houses[];
    private static byte cities[];
    private static byte roads[];

    @SuppressWarnings("unused")
    private Action2<Byte, Player> onDevcardChange;
    private List<Runnable> onInventoryChange;

    private static List<Action<Byte>> onTurnIDChange;

    static {
        houses = new byte[Constants.VERTECIES];
        cities = new byte[Constants.VERTECIES];
        roads = new byte[Constants.LINES];

        onTurnIDChange = new ArrayList<>();

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
        this.devcards = new byte[5];
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

        count += devcards[1];

        return count;
    }

    public byte getLongestRoadLength() {
        return 0;
    }

    public byte getDevCardsCount() {

        byte sum = 0;
        for (int i = 0; i < devcards.length; i++) {
            sum += devcards[i];
        }
        return sum;
    }

    public Color getColor() {
        return color;
    }

    public byte[] getMaterials() {
        return this.materials;
    }

    protected void setMaterials(byte[] materials) {
        System.out.println("got materials =" + Arrays.toString(materials));
        this.materials = materials;
        callOnInventoryChange();
    }

    protected void setDevelopements(byte[] devcards) {
        this.devcards = devcards;
        callOnInventoryChange();
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

    public static void addOnTurnIDChange(Action<Byte> handler) {
        onTurnIDChange.add(handler);
    }

    public static void removeOnTurnIDChange(Action<Byte> handler) {
        onTurnIDChange.remove(handler);
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
        turnID = (byte) Math.min(Math.max(id, 0), 2);

        for (var event : onTurnIDChange) {
            event.action(turnID);
        }
    }

    protected static void moveTurn() {
        setTurnID((byte) (turnID + 1));
    }
}