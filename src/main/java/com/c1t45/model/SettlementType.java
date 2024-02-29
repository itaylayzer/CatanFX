package com.c1t45.model;

public enum SettlementType {
    none((byte) 0),
    house((byte) 1),
    city((byte) 2);

    private byte value;

    private SettlementType(byte value) {
        this.value = (byte) value;
    }

    public byte getValue() {
        return this.value;
    }
}
