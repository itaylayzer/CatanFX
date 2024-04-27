import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("localhost", 8080);
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        BufferedReader consoleInput = new BufferedReader(new InputStreamReader(System.in));
        
        System.out.println("Enter message to send to server:");
        String message = consoleInput.readLine();
        out.println(message);
        
        String response = in.readLine();
        System.out.println("Response from server: " + response);
        
        socket.close();
    }
}
