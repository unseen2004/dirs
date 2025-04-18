package org.example.zad;

import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.ColorPicker;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.scene.shape.Shape;


/**
 * Klasa reprezentująca okno dialogowe do wyboru koloru dla wybranej figury.
 */
public class FXColorPicker {
    /**
     * Wyświetla okno dialogowe do wyboru koloru na podanej pozycji.
     * @param ownerStage główne okno aplikacji.
     * @param x pozycja x okna dialogowego.
     * @param y pozycja y okna dialogowego.
     * @param shape figura, dla której kolor ma być zmieniony.
     */
    public void show(Stage ownerStage, double x, double y, Shape shape) {

        ColorPicker picker = new ColorPicker();

        picker.setOnAction(e -> shape.setFill(picker.getValue()));

        VBox vbox = new VBox();
        vbox.setSpacing(20);
        vbox.setPadding(new Insets(25, 50, 50, 60));
        vbox.getChildren().addAll(picker);

        Stage stage = new Stage();

        // Ustawienie właściciela okna dialogowego
        stage.initOwner(ownerStage);

        stage.initModality(Modality.WINDOW_MODAL);

        //Ustawienie pozycji okna na podstawie współrzędnych kursora
        stage.setX(x + 10);
        stage.setY(y);

        Scene scene = new Scene(vbox, 200, 200, Color.BEIGE);
        stage.setTitle("Kolor/rotacja");
        stage.setScene(scene);
        stage.show();
    }
}
