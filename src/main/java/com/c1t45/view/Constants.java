package com.c1t45.view;

import com.c1t45.Main;
import com.c1t45.view.Packages.ImagePackage;

import javafx.scene.image.Image;
import javafx.scene.paint.Paint;

public class Constants {
        static public final byte dots = 54;
        static public final byte lines = 72;

        public enum Materials {
                wood((byte) 0),
                wool((byte) 1),
                wheet((byte) 2),
                brick((byte) 3),
                ore((byte) 4);

                private byte value;

                private Materials(byte value) {
                        this.value = value;
                }

                public byte getValue() {
                        return value;
                }

        };

        public class Images {
                public static final Image wood = new Image(url("icons/materials/wood3.png")),
                                wool = new Image(url("icons/materials/wool3.png")),
                                wheet = new Image(url("icons/materials/wheet3.png")),
                                brick = new Image(url("icons/materials/brick.png")),
                                ore = new Image(url("icons/materials/ore.png")),
                                mat = new Image(url("icons/stats/mat.png")),
                                devcards = new Image(url("icons/stats/devcards.png")),
                                house = new Image(url("icons/stats/house.png")),
                                points = new Image(url("icons/stats/points.png")),
                                roads = new Image(url("icons/stats/road4.png")),
                                city = new Image(url("icons/stats/city.png")),
                                trade = new Image(url("icons/stats/trade.png")),
                                timer = new Image(url("icons/stats/timer.png")),
                                rolldice = new Image(url("icons/stats/rolldices.png")),
                                icon = new Image(url("icons/icon/bircksicon.png"));
                public static final Image[] dices = new Image[] {
                                new Image(url("icons/dices/c1.png")),
                                new Image(url("icons/dices/c2.png")),
                                new Image(url("icons/dices/c3.png")),
                                new Image(url("icons/dices/c4.png")),
                                new Image(url("icons/dices/c5.png")),
                                new Image(url("icons/dices/c6.png"))
                };

                private static String url(String local) {
                        return Main.class.getResource(local).toString();
                }
        }

        public class Packages {

                public static final ImagePackage[] materials = new ImagePackage[] {
                                new ImagePackage("wood", Paint.valueOf("#75d9a7"), Images.wood),
                                new ImagePackage("wool", Paint.valueOf("#a0f38b"), Images.wool),
                                new ImagePackage("wheet", Paint.valueOf("#f1ee8f"), Images.wheet),
                                new ImagePackage("brick", Paint.valueOf("#e98a84"), Images.brick),
                                new ImagePackage("ore", Paint.valueOf("#d0d0d0"), Images.ore),
                };

                public static final ImagePackage material = new ImagePackage("materials", Images.mat),
                                devcards = new ImagePackage("development cards", Images.devcards),
                                settlements = new ImagePackage("settlements", Images.house),
                                points = new ImagePackage("victory points", Images.points),
                                roads = new ImagePackage("roads", Images.roads),
                                city = new ImagePackage("city", Images.city),
                                house = new ImagePackage("house", Images.house),
                                trade = new ImagePackage("trade", Images.trade),
                                endturn = new ImagePackage("end turn", Images.timer),
                                rolldice = new ImagePackage("Roll Dices", Images.rolldice);
        }

        public class Store {
                public static final byte[] road = { 1, 0, 0, 1, 0 },
                                house = { 1, 1, 1, 1, 0 },
                                city = { 0, 0, 2, 0, 3 },
                                developmentCard = { 0, 1, 1, 0, 1 };
        }
}
