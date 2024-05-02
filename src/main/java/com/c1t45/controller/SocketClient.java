package com.c1t45.controller;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import com.c1t45.controller.Constants.ClientCodes;
import com.c1t45.controller.Constants.ServerCodes;
import com.c1t45.view.LocalPlayer;

public class SocketClient {
    private Socket socket;

    static private final String ip = "127.0.0.1";
    static private final int port = 5900;

    private LocalPlayer player;

    public SocketClient(LocalPlayer localPlayer) throws UnknownHostException, IOException {
        this.player = localPlayer;
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

    private byte[] recv() throws IOException, Exception {
        DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());

        int size = dataInputStream.readByte();
        byte[] data = new byte[size];
        int bytesRead = dataInputStream.read(data, 0, size);

        if (bytesRead != size)
            throw new Exception("bytesRead (" + bytesRead + ") != size (" + size + ")");

        return data;

    }

    private byte[] action(byte[] data) throws IOException, Exception {
        this.send(data);
        return this.recv();
    }

    public byte[] getLands() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.LANDS });
    }

    public byte[] getHarbors() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.HARBORS });
    }

    public byte[] rollDice() throws IOException, Exception {
        byte rolled = this.action(new byte[] { ClientCodes.ACTIONS.DICE })[0];
        return new byte[] { (byte) (rolled & 0x0F), (byte) (rolled >> 4) };
    }

    public void close() throws IOException {
        if (socket != null) {
            socket.close();
        }
    }

    public void mainLoop() throws IOException, Exception {
        switch (this.recv()[0]) {
            case ServerCodes.TURN:
                player.setActionable(this.action(new byte[] { ClientCodes.INFORMATION.PLAYER })[0]);
                break;
            case ServerCodes.UPDATE:
                break;
            case ServerCodes.PUT:
                break;
        }
    }

}
