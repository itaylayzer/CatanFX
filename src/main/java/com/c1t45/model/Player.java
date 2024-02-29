package com.c1t45.model;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Arrays;

import com.c1t45.model.Interfaces.Action2;
import com.c1t45.model.Interfaces.Predicate;
import com.c1t45.model.Utils.BytesUtils;
import com.c1t45.model.Utils.LandUtils;
import com.c1t45.view.Constants;
import com.c1t45.view.Constants.Materials;

import javafx.scene.paint.Color;

public abstract class Player {

    protected byte id;
    protected String name;
    protected byte[] materials;
    private Color color;

    private static byte[] lands;

    private List<Object> devCards;
    private static byte houses[];
    private static byte cities[];
    private static byte roads[];

    // global listeners
    private static List<Action2<Byte, Player>> onHousesChange;
    private static List<Action2<Byte, Player>> ohCitiesChange;
    private static List<Action2<Byte, Player>> onRoadsChange;
    private static List<Action2<Byte, Byte>> onRollAction;

    @SuppressWarnings("unused")
    private Action2<Byte, Player> onDevcardChange;
    private List<Runnable> onInventoryChange;

    static {
        houses = new byte[Constants.dots];
        cities = new byte[Constants.dots];
        roads = new byte[Constants.lines];

        onHousesChange = new ArrayList<>();
        ohCitiesChange = new ArrayList<>();
        onRoadsChange = new ArrayList<>();
        onRollAction = new ArrayList<>();

    }

    public static void initialize(byte[] landsPackage) {
        lands = landsPackage;
    }

    public static byte[] getHouses() {
        return houses;
    }

    public static byte[] getCities() {
        return cities;
    }

    public static void addOnHouseChange(Action2<Byte, Player> action) {
        onHousesChange.add(action);
    }

    public static void addOnCityChange(Action2<Byte, Player> action) {
        ohCitiesChange.add(action);
    }

    public static void addOnRoadChange(Action2<Byte, Player> action) {
        onRoadsChange.add(action);
    }

    public static void addOnRoll(Action2<Byte, Byte> action) {
        onRollAction.add(action);
    }

    public static void removeOnHouseChange(Action2<Byte, Player> action) {
        onHousesChange.remove(action);
    }

    public static void removeOnCityChange(Action2<Byte, Player> action) {
        ohCitiesChange.remove(action);
    }

    public static void removeOnRoadChange(Action2<Byte, Player> action) {
        onRoadsChange.remove(action);
    }

    public static void removeOnRoll(Action2<Byte, Byte> action) {
        onRollAction.remove(action);
    }

    private static byte[] rollMaterials(byte player, byte roll) {
        byte[] materials = new byte[5];
        byte index;

        List<Byte> hexesIndex = new ArrayList<>();
        List<Byte> hexesMaterials = new ArrayList<>();

        for (index = 0; index < 18; index += 2) {
            byte material = lands[index];
            byte number = lands[index + 1];

            byte landIndex = (byte) (index / 2);

            if (landIndex > 8)
                landIndex++;

            if (number == roll) {
                hexesIndex.add(landIndex);
                hexesMaterials.add(material);
            }
        }

        System.out.println("\tlands = " + Arrays.toString(hexesIndex.toArray()));

        Iterator<Byte> iterator = hexesMaterials.iterator();

        for (byte landIndex : hexesIndex) {
            System.out.println("\t\tland index = " + landIndex);

            byte count = 0;
            byte material = iterator.next();

            byte[] vertecies = LandUtils.indexToVertecis(landIndex);
            System.out.println("\t\t\tland vertecies = " + Arrays.toString(vertecies));

            for (byte vertex : vertecies) {
                count += (byte) ((houses[vertex] == player ? 1 : 0) + (cities[vertex] == player ? 1 : 0));
            }

            materials[material] += count;
        }

        return materials;
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

        for (index = 0; index < Constants.dots; index++) {
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

    public byte buyHouse(byte house) {

        if (houses[house] != 0)
            return (byte) 1;

        if (!enoughMaterials(Constants.Store.house))
            return (byte) 2;

        houses[house] = this.id;
        BytesUtils.minus(this.materials, Constants.Store.house);

        for (Action2<Byte, Player> event : onHousesChange) {
            event.action(house, this);
        }

        callOnInventoryChange();
        return (byte) 0;
    }

    public byte buyCity(byte city) {
        if (houses[city] != this.id)
            return (byte) 1;

        if (!enoughMaterials(Constants.Store.city))
            return (byte) 2;

        cities[city] = this.id;

        BytesUtils.minus(this.materials, Constants.Store.city);

        for (Action2<Byte, Player> event : ohCitiesChange) {
            event.action(city, this);
        }

        callOnInventoryChange();
        return (byte) 0;
    }

    public byte buyDevCard() {
        if (!enoughMaterials(Constants.Store.developmentCard))
            return (byte) 1;

        BytesUtils.minus(this.materials, Constants.Store.developmentCard);

        // if (onDevcardChange != null)
        // onDevcardChange.action(null, null);();

        callOnInventoryChange();
        return (byte) 0;
    }

    public byte buyRoad(byte road) {

        if (roads[road] != 0)
            return (byte) 1;

        if (!enoughMaterials(Constants.Store.road))
            return (byte) 2;

        roads[road] = this.id;
        BytesUtils.minus(this.materials, Constants.Store.road);

        for (Action2<Byte, Player> event : onRoadsChange) {
            event.action(road, this);
        }

        callOnInventoryChange();
        return (byte) 0;
    }

    public static boolean houseDontBelong(Byte value) {
        return houses[value] == 0;
    }

    public static boolean roadDontBelong(Byte value) {
        return roads[value] == 0;
    }

    public boolean hasHouse(Byte index) {
        return houses[index] == this.id;
    }

    public boolean hasCity(Byte index) {
        return cities[index] == this.id;
    }

    public void rollAction(Byte c1, Byte c2) {
        for (Action2<Byte, Byte> event : onRollAction) {
            event.action(c1, c2);
        }

        byte result = (byte) (c1 + c2);
        switch (result) {
            case 7:
                // move the knight;
                break;
            default:
                byte[] addedMaterials = Player.rollMaterials(this.id, result);
                System.out.println("result: " + result + "\naddded materials: " + Arrays.toString(addedMaterials));
                addMaterials(addedMaterials);
                // notify the user with notification, like tomatojs or something like that
                break;

        }

        callOnInventoryChange();
    }

}