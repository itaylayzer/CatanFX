package com.c1t45.controller;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketClient {
    private Socket socket;

    static private final String ip = "129.159.143.67";
    static private final int port = 1050;

    public SocketClient() throws UnknownHostException, IOException {
        this.socket = new Socket(ip, port);

    }

    private void send(byte[] msg) {
        try {
            OutputStream outputStream = socket.getOutputStream();
            outputStream.write(msg);
            outputStream.flush();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private byte[] recv() {
        try {
            DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());

            int size = dataInputStream.readByte();
            byte[] data = new byte[size];
            int bytesRead = dataInputStream.read(data, 0, size);

            if (bytesRead != size)
                throw new Exception("bytesRead (" + bytesRead + ") != size (" + size + ")");

            return data;

        } catch (Exception ex) {
            ex.printStackTrace();
            return null;
        }

    }

    public byte[] getLands() {
        this.send(new byte[] { (byte) 0 });
        return this.recv();
    }

    public byte[] getHarbors() {
        this.send(new byte[] { (byte) 1 });
        return this.recv();
    }

    public byte[] rollDice() {
        this.send(new byte[] { (byte) 20 });
        byte rolled = this.recv()[0];

        return new byte[] { (byte) (rolled & 0x0F), (byte) (rolled >> 4) };
    }

    public void close() throws IOException {
        if (socket != null) {
            socket.close();
        }
    }

}
