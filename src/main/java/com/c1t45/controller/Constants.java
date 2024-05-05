package com.c1t45.controller;

public class Constants {

    public static class ClientCodes {
        public static class INFORMATION {
            public static final byte LANDS = 0, HARBORS = 1, PLAYER = 10, MATS = 11, DEVCARDS = 12, AMOUNTS = 14;
        }

        public static class ACTIONS {
            public static final byte DICE = 30, STORE = 31, END_TURN = 40;
        }
    }

    public static class ServerCodes {
        public static final byte TURN = 0, UPDATE = 1, PUT = 2;
    }

}
