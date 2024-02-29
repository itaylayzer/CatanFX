package com.c1t45.model.Generators;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import com.c1t45.model.Utils.BytesUtils;

public class LandsGenerator implements Generatable<byte[]> {

    @Override
    public byte[] generate() throws IndexOutOfBoundsException {
        int index = 0;
        // Define the distribution of resources
        List<Byte> materialList = new ArrayList<>();
        materialList.addAll(Collections.nCopies(4, (byte) 0)); // Forest
        materialList.addAll(Collections.nCopies(4, (byte) 1)); // Pasture
        materialList.addAll(Collections.nCopies(4, (byte) 2)); // Fields
        materialList.addAll(Collections.nCopies(3, (byte) 3)); // Hills
        materialList.addAll(Collections.nCopies(3, (byte) 4)); // Mountains

        final int size = materialList.size();
        // Shuffle the resources array
        Collections.shuffle(materialList);

        List<Byte> numbers = Arrays
                .asList(new Byte[] { 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12 });

        // Shuffle the numbers array
        Collections.shuffle(numbers);
        List<Byte> landsList = new ArrayList<>();
        // first the material and the the number
        for (index = 0; index < size; index++) {
            landsList.add(materialList.get(index));
            landsList.add(numbers.get(index));
        }


        return BytesUtils.listToArray(landsList);
    }

}
