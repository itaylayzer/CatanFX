package com.itayl.view.Generators;

import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import com.itayl.view.Interfaces.Generatable;

public class NameGenerator implements Generatable<String> {

    private static String[] constantNames = { "Eizenberg", "Trumer", "Huffman", "Oppenhiemer" };
    private Iterator<String> names;

    public NameGenerator() {
        List<String> listNames = Arrays.asList(constantNames);
        Collections.shuffle(listNames);
        this.names = listNames.iterator();
    }

    @Override
    public String generate() throws IndexOutOfBoundsException {
        return this.names.next();
    }

}
