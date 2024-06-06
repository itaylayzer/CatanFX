package com.itayl.controller;

public class Constants {

    public static class ClientCodes {
        public static class INFORMATION {
            public static final byte LANDS = 0, HARBORS = 1, NEARBY = 2, PLAYER = 3, MATS = 4, DEVCARDS = 5,
                    AMOUNTS = 7;
        }

        public static class ACTIONS {
            public static final byte DICE = 9, STORE = 10, END_TURN = 14, ROBBER = 11, USE_DEV_CARD = 12, DROP = 13;
        }
    }

    public static class ServerCodes {
        public static final byte TURN = 0, UPDATE_SETTLEMENT = 1, UPDATE_ROAD = 2, DICE = 3;
    }

}
