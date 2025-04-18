import java.net.*;
import java.io.*;

public class Client {

    public static void main(String[] args) {
        try {
            Socket socket = new Socket("localhost", 4444);
            // Sending to server
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            // Receiving from server
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            Console console = System.console();
            String text;

            do {
                text = console.readLine("Enter text: ");
                // Sending to server
                out.println(text);
                // Receiving from server
                System.out.println(in.readLine());
            } while (!text.equals("bye"));

            socket.close();
        } catch (UnknownHostException ex) {
            System.out.println("Server not found: " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("I/O error: " + ex.getMessage());
        }
    }
}