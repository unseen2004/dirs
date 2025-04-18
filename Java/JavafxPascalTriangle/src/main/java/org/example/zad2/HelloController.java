package org.example.zad2;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class HelloController {
    @FXML
    private Label area;

    @FXML
    private TextField input;

    private Stage window;

    public void setWindow(Stage window) {
        this.window = window;
    }

    @FXML
    protected void onRunClicked() {
        String inputText = input.getText();
        if (inputText == null || inputText.isEmpty()) {
            System.out.println("Text field is empty");
            return;
        }
        try {
            int n = Integer.parseInt(inputText);
            if (n < 1) {
                System.out.println("n is too small");
                return;
            }
            PascalTriangle triangle = new PascalTriangle(n);
            String result = triangle.getResult();
            int windowX = 27 * n * 2, windowY = 22 * n;
            if (windowX < 400 && windowY < 400) {
                window.setWidth(400);
                window.setHeight(400);
                input.setPrefWidth(300);
                input.setPrefHeight(100);
                area.setPrefWidth(400);
                area.setPrefHeight(300);
                area.setText(result);
            } else if (windowX < 400 && windowY >= 400) {
                window.setWidth(400);
                window.setHeight(windowY);
                input.setPrefWidth(300);
                input.setPrefHeight(100);
                area.setPrefWidth(400);
                area.setPrefHeight(windowY - 100);
                area.setText(result);
            } else if (windowX >= 400 && windowY < 400) {
                window.setWidth(windowX);
                window.setHeight(400);
                input.setPrefWidth(windowX - 100);
                input.setPrefHeight(100);
                area.setPrefWidth(windowX);
                area.setPrefHeight(300);
                area.setText(result);
            } else {
                window.setWidth(windowX);
                window.setHeight(windowY);
                input.setPrefWidth(windowX - 100);
                input.setPrefHeight(100);
                area.setPrefWidth(windowX);
                area.setPrefHeight(windowY - 100);
                area.setText(result);
            }
        } catch (NumberFormatException ex) {
            System.out.println("Invalid number format");
        }
    }
}