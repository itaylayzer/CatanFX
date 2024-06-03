package com.itayl.controller;

public class Constants {

    public static class ClientCodes {
        public static class INFORMATION {
            public static final byte LANDS = 0, HARBORS = 1, NEARBY = 2, PLAYER = 10, MATS = 11, DEVCARDS = 12,
                    AMOUNTS = 14;
        }

        public static class ACTIONS {
            public static final byte DICE = 30, STORE = 31, END_TURN = 40, ROBBER = 32, USE_DEV_CARD = 33, DROP = 34;
        }
    }

    public static class ServerCodes {
        public static final byte TURN = 0, UPDATE = 1, PUT = 2;
    }

}