package com.c1t45.controller;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import com.c1t45.controller.Constants.ClientCodes;
import com.c1t45.controller.Constants.ServerCodes;
import com.c1t45.view.Player;
import com.c1t45.view.Interfaces.Action;
import com.c1t45.view.Interfaces.Condition;

public class SocketClient {
    private Socket socket;

    static private final String ip = "127.0.0.1";
    static private final int port = 5900;

    public SocketClient() throws UnknownHostException, IOException {
        this.socket = new Socket(ip, port);

    }

    private void send(byte[] msg) {
        try {
            OutputStream outputStream = socket.getOutputStream();
            outputStream.write(msg);
            outputStream.flush();
        } catch (Exception ex) {
            ex.printStackTrace(System.err);
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

    public byte getActionable() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.PLAYER })[0];
    }

    public boolean storeRoad(byte from, byte to) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.STORE, 0, from, to })[0] != 1;
    }

    public boolean storeHouse(byte vertex) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.STORE, 1, vertex })[0] != 1;
    }

    public boolean storeCity(byte vertex) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.STORE, 2, vertex })[0] != 1;
    }

    public void endTurn(Condition<Void> condition, Action<Void> onDone) throws IOException, Exception {
        this.send(new byte[] { ClientCodes.ACTIONS.END_TURN });

        Thread thread = new Thread(() -> {
            System.out.println("starting thread!");
            while (condition.condition(null)) {
                try {
                    System.out.println("before recv()");
                    byte[] response = this.recv();
                    System.out.println("after recv()");

                    switch (response[0]) {
                        case ServerCodes.TURN:
                            Player.setTurnID(response[1]);
                            break;
                    }
                } catch (Exception ex) {
                    ex.printStackTrace(System.err);
                }
            }
            try {
                this.recv();
                onDone.action(null);
            } catch (Exception ex) {
                System.err.println("Last Recv before returning to local turn!");
                ex.printStackTrace(System.err);
            }

        });
        thread.start();

    }

}
