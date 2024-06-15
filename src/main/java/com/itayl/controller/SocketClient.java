package com.itayl.controller;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import com.itayl.controller.Constants.ClientCodes;
import com.itayl.view.Constants.Actions.TransferMaterials;
import com.itayl.view.Interfaces.Action;
import com.itayl.view.Interfaces.Condition;

public class SocketClient {
    private Socket socket;

    static private final String IP = "127.0.0.1";
    static private final int PORT = 5173;

    public SocketClient() throws UnknownHostException, IOException {
        this.socket = new Socket(IP, PORT);
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

    public byte[] getActionable() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.PLAYER, 0 });
    }

    public byte storeRoad(boolean transferMaterials, byte from, byte to) throws IOException, Exception {
        return this.action(
                new byte[] { ClientCodes.ACTIONS.STORE, 0, (byte) (transferMaterials ? 1 : 0), from, to })[0];
    }

    public boolean storeHouse(TransferMaterials transferMaterials, byte vertex) throws IOException, Exception {
        return this.action(
                new byte[] { ClientCodes.ACTIONS.STORE, 1, transferMaterials.getValue(), vertex })[0] != 1;
    }

    public boolean storeCity(byte vertex) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.STORE, 2, vertex })[0] != 1;
    }

    public boolean storeDevCard() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.STORE, 3 })[0] != 1;
    }

    public void endTurn(Condition<Void> condition, Action<byte[]> handle, Action<Void> onDone)
            throws IOException, Exception {
        this.send(new byte[] { ClientCodes.ACTIONS.END_TURN });

        Thread thread = new Thread(() -> {
            while (condition.condition(null)) {
                try {
                    byte[] response = this.recv();

                    handle.action(response);

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

    public byte[] getMaterials(byte id) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.MATS, (byte) (id - 1) });
    }

    public byte[] getDevelopments(byte id) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.DEVCARDS, (byte) (id - 1) });
    }

    public byte[] getAmounts() throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.AMOUNTS });
    }

    public byte moveRobber(byte robber, byte playerIndex) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.ROBBER, playerIndex, robber })[0];
    }

    public byte[] usedDevCard(byte offset) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.USE_DEV_CARD, offset });
    }

    public byte[] yearOfPlant(Byte firstMat, Byte secondMat) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.USE_DEV_CARD, 3, firstMat, secondMat });
    }

    public byte[] monopol(Byte matIndex) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.ACTIONS.USE_DEV_CARD, 4, matIndex });
    }

    public byte nearbyPlayers(byte picked) throws IOException, Exception {
        return this.action(new byte[] { ClientCodes.INFORMATION.NEARBY, picked })[0];
    }

    public void removeMats(Byte[] matsToDrop, boolean notifyBots) {

        try {
            this.action(
                    new byte[] { ClientCodes.ACTIONS.DROP, 0, (byte) (notifyBots ? 1 : 0), matsToDrop[0], matsToDrop[1],
                            matsToDrop[2],
                            matsToDrop[3], matsToDrop[4] });
        } catch (Exception exp) {
            exp.printStackTrace(System.err);
        }

    }

    public void deal(byte dealType) throws IOException, Exception {
        this.action(new byte[] { ClientCodes.ACTIONS.DEAL, dealType });
    }

    public void endGame() {
        this.send(new byte[] { ClientCodes.ACTIONS.END_GAME });
    }
}
