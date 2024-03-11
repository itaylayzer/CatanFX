package com.c1t45.model;

import com.c1t45.controller.Observable;
import com.c1t45.model.Interfaces.UtilAction;
import com.c1t45.model.Utils.BytesUtils;

public class Inventory extends Observable<byte[]> {
    private byte[] materials;

    public Inventory() {
        materials = new byte[5];
    }

    private void modifyMaterials(byte[] operand, UtilAction<byte[]> action) {
        byte[] previusValue = BytesUtils.duplicate(materials);
        materials = action.action(materials, operand);
        notifyObservers(previusValue, materials);
    }

    public void add(byte[] operand) {
        modifyMaterials(operand, new UtilAction<byte[]>() {
            public byte[] action(byte[] firstOperand, byte[] secondOperand) {
                return BytesUtils.add(firstOperand, secondOperand);
            }
        });
    }

    public void subtruct(byte[] operand) {
        modifyMaterials(operand, new UtilAction<byte[]>() {
            public byte[] action(byte[] firstOperand, byte[] secondOperand) {
                return BytesUtils.minus(firstOperand, secondOperand);
            }
        });
    }

    public byte getCount() {
        byte materialsCount = 0;

        for (byte mat : materials) {
            materialsCount += mat;
        }

        return materialsCount;
    }
}
