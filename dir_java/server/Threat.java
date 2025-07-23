import java.io.*;
import java.net.Socket;

public class Threat implements Runnable {
    private Socket socket;
    private String treeType;
    private Tree<Integer> integerTree;
    private Tree<Double> doubleTree;
    private Tree<String> stringTree;

    public Threat(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            // Receiving from socket
            InputStream input = socket.getInputStream();
            BufferedReader in = new BufferedReader(new InputStreamReader(input));

            // Sending to socket
            OutputStream output = socket.getOutputStream();
            PrintWriter out = new PrintWriter(output, true);

            String line;
            while ((line = in.readLine()) != null) {
                String[] command = line.split(" ");
                if (command[0].equalsIgnoreCase("bye")) {
                    out.println("bye");
                } else {
                    String response = executeCommand(command);
                    // Sending to socket
                    out.println(response);
                }
                if (command[0].equals("bye")) {
                    break;
                }
            }

            socket.close();
        } catch (IOException ex) {
            System.out.println("Server exception: " + ex.getMessage());
            ex.printStackTrace();
        }
    }

    private String executeCommand(String[] command) {
        switch (command[0].toLowerCase()) {
            case "insert":
                return insertElement(command);
            case "delete":
                return deleteElement(command);
            case "search":
                return searchElement(command);
            case "draw":
                return drawTree();
            case "type":
                return setType(command);
            default:
                return "Unknown command";
        }
    }

    private String insertElement(String[] command) {
        if (command.length < 2) return "Missing value";
        try {
            switch (treeType) {
                case "Integer":
                    integerTree.insert(Integer.parseInt(command[1]));
                    return "Element inserted: " + command[1];
                case "Double":
                    doubleTree.insert(Double.parseDouble(command[1]));
                    return "Element inserted: " + command[1];
                case "String":
                    stringTree.insert(command[1]);
                    return "Element inserted: " + command[1];
                default:
                    return "Unknown tree type";
            }
        } catch (NumberFormatException e) {
            return "Invalid number format";
        }
    }

    private String deleteElement(String[] command) {
        if (command.length < 2) return "Missing value";
        try {
            switch (treeType) {
                case "Integer":
                    integerTree.delete(Integer.parseInt(command[1]));
                    return "Element deleted: " + command[1];
                case "Double":
                    doubleTree.delete(Double.parseDouble(command[1]));
                    return "Element deleted: " + command[1];
                case "String":
                    stringTree.delete(command[1]);
                    return "Element deleted: " + command[1];
                default:
                    return "Unknown tree type";
            }
        } catch (NumberFormatException e) {
            return "Invalid number format";
        }
    }

    private String searchElement(String[] command) {
        if (command.length < 2) return "Missing value";
        switch (treeType) {
            case "Integer":
                return integerTree.isElement(Integer.parseInt(command[1])) ? "Element found" : "Element not found";
            case "Double":
                return doubleTree.isElement(Double.parseDouble(command[1])) ? "Element found" : "Element not found";
            case "String":
                return stringTree.isElement(command[1]) ? "Element found" : "Element not found";
            default:
                return "Unknown tree type";
        }
    }

    private String drawTree() {
        switch (treeType) {
            case "Integer":
                return integerTree.toString();
            case "Double":
                return doubleTree.toString();
            case "String":
                return stringTree.toString();
            default:
                return "Unknown tree type";
        }
    }

    private String setType(String[] command) {
        if (command.length < 2) return "Missing tree type";
        switch (command[1].toLowerCase()) {
            case "integer":
                treeType = "Integer";
                return "Tree type set to Integer";
            case "double":
                treeType = "Double";
                return "Tree type set to Double";
            case "string":
                treeType = "String";
                return "Tree type set to String";
            default:
                return "Unknown tree type";
        }
    }
}