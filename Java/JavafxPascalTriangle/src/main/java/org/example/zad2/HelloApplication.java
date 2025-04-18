package org.example.zad2;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

class PascalTriangle {
    int n;

    public PascalTriangle(int n) {
        this.n = n;
    }

    public String getResult() {
        if (n == 1) {
            return "1";
        }

        int[][] t = new int[n][];
        for (int i = 0; i < n; i++) {
            t[i] = new int[i + 1];
            for (int j = 0; j < i + 1; j++) {
                if (j == 0 || j == i) {
                    t[i][j] = 1;
                } else {
                    t[i][j] = t[i - 1][j - 1] + t[i - 1][j];
                }
            }
        }

        String result = "";
        String lastRow = "";
        for (int i = 0; i < n; i++) {
            lastRow += Integer.toString(t[n - 1][i]) + " ";
        }
        int lastRowLength = lastRow.length();
        for (int i = 0; i < n; i++) {
            String row = "";
            for (int j = 0; j < i + 1; j++) {
                row += Integer.toString(t[i][j]) + " ";
            }
            int padding = (lastRowLength - row.length()) / 2;
            String tmp = "";
            for (int x = 0; x < padding; x++) {
                tmp += " ";
            }
            tmp += row;
            result += tmp + "\n";
        }
        return result;
    }
}

public class HelloApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));
        Scene window = new Scene(fxmlLoader.load(), 400, 400);
        stage.setTitle("Pascal's Triangle");
        stage.setScene(window);
        stage.show();

        HelloController controller = fxmlLoader.getController();
        controller.setWindow(stage);
    }

    public static void main(String[] args) {
        launch();
    }
}