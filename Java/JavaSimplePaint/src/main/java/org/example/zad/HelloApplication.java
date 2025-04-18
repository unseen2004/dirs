package org.example.zad;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import javafx.geometry.Pos;
import javafx.scene.shape.Shape;
import javafx.scene.transform.Rotate;
import javafx.geometry.Bounds;

/**
 * Główna klasa aplikacji
 */
public class HelloApplication extends Application {
    private String selectedShape = null;
    private Pane mainArea;
    private static Shape activeShape;
    private static Stage stage;

    /**
     * @return stage.
     */
    public static Stage getStage() {
        return stage;
    }

    /**
     * @return activeShape.
     */
    public static Shape getActiveShape() {
        return activeShape;
    }

    /**
     * Ustawia aktywny kształt.
     * @param shape kształt do ustawienia jako aktywny.
     */
    public static void setActiveShape(Shape shape) {
        if (activeShape != null) {
            if (activeShape instanceof FXRectangle) {
                ((FXRectangle) activeShape).removeBorder();//rzutowanie/konwertowanie na typ FXRectangle
            } else if (activeShape instanceof FXCircle) {
                ((FXCircle) activeShape).removeBorder();
            } else if (activeShape instanceof FXTriangle) {
                ((FXTriangle) activeShape).removeBorder();
            }
        }
        activeShape = shape;
        if (activeShape != null) {
            if (activeShape instanceof FXRectangle) {
                ((FXRectangle) activeShape).addBorder();
                ((FXRectangle) activeShape).toFront();
            } else if (activeShape instanceof FXCircle) {
                ((FXCircle) activeShape).addBorder();
                ((FXCircle) activeShape).toFront();
            } else if (activeShape instanceof FXTriangle) {
                ((FXTriangle) activeShape).addBorder();
                ((FXTriangle) activeShape).toFront();
            }
            //activeShape.requestFocus();
        }
    }

    /**.
     * @param stage główny obiekt Stage dla tej aplikacji.
     */
    @Override
    public void start(Stage stage) {
        HelloApplication.stage = stage; // Ustawienie pola stage

        // Główny obszar aplikacji
        mainArea = new Pane();
        mainArea.setMinWidth(1000);
        mainArea.setMinHeight(800);

        // Margines po prawej stronie
        VBox rightMargin = new VBox();
        rightMargin.setMinWidth(200);
        rightMargin.setPadding(new Insets(10)); // Margines wewnętrzny
        rightMargin.setSpacing(10); //Odstęp między children VBox
        rightMargin.setBackground(new Background(new BackgroundFill(Color.LIGHTGRAY, CornerRadii.EMPTY, Insets.EMPTY)));
        rightMargin.setAlignment(Pos.TOP_CENTER);

        // Dodanie napisu "Menu" na górze marginesu
        Label menuLabel = new Label("Menu");
        menuLabel.setFont(new Font("Arial", 24));
        menuLabel.setTextFill(Color.BLUE);

        // Dodanie napisu do HBox
        HBox menuContainer = new HBox(menuLabel);
        menuContainer.setAlignment(Pos.CENTER);

        // Dodanie HBox do VBox
        rightMargin.getChildren().add(menuContainer);

        // Dodanie ToggleGroup dla przycisków kształtów
        ToggleGroup ButtonsToggleGroup = new ToggleGroup();

        //Dodanie przycisków poniżej HBox
        ToggleButton okragButton = new ToggleButton("Okrąg");
        ToggleButton prostokatButton = new ToggleButton("Prostokąt");
        ToggleButton trojkatButton = new ToggleButton("Trójkąt");
        Button infoButton = new Button("Info");
        Button obrotButton = new Button("Obróć");

        //Dodanie przycisków do ToggleGroup
        okragButton.setToggleGroup(ButtonsToggleGroup);
        prostokatButton.setToggleGroup(ButtonsToggleGroup);
        trojkatButton.setToggleGroup(ButtonsToggleGroup);

        //Ustawienie szerokości przycisków
        okragButton.setMaxWidth(Double.MAX_VALUE);
        prostokatButton.setMaxWidth(Double.MAX_VALUE);
        trojkatButton.setMaxWidth(Double.MAX_VALUE);
        infoButton.setMaxWidth(Double.MAX_VALUE);
        obrotButton.setMaxWidth(Double.MAX_VALUE);

        //Ustawienie preferowanej wysokości przycisków
        okragButton.setPrefHeight(100);
        prostokatButton.setPrefHeight(100);
        trojkatButton.setPrefHeight(100);
        infoButton.setPrefHeight(100);
        obrotButton.setPrefHeight(100);

        // Ustawienie czcionki dla przycisków
        Font buttonFont = new Font("Arial", 18);
        okragButton.setFont(buttonFont);
        prostokatButton.setFont(buttonFont);
        trojkatButton.setFont(buttonFont);
        infoButton.setFont(buttonFont);
        obrotButton.setFont(buttonFont);

        // Obsługa kliknięcia przycisku Info
        infoButton.setOnAction(e -> showInfoWindow());

        obrotButton.setOnAction(e -> {
            if (activeShape == null) {
                return;
            }
            Bounds bounds = activeShape.getBoundsInLocal();
            double pivotX = bounds.getMinX() + bounds.getWidth() / 2;
            double pivotY = bounds.getMinY() + bounds.getHeight() / 2;
            Rotate rotate = new Rotate(45, pivotX, pivotY);
            activeShape.getTransforms().add(rotate);
        });



        // Ustawienie wyboru kształtu
        okragButton.setOnAction(e -> selectedShape = okragButton.isSelected() ? "Okrąg" : null);
        prostokatButton.setOnAction(e -> selectedShape = prostokatButton.isSelected() ? "Prostokąt" : null);
        trojkatButton.setOnAction(e -> selectedShape = trojkatButton.isSelected() ? "Trójkąt" : null);

        // Dodanie przycisków do VBox
        rightMargin.getChildren().addAll(okragButton, prostokatButton, trojkatButton, infoButton, obrotButton);

        // Obsługa kliknięcia na panelu głównym (dla wyłączenia aktywności figur)
        mainArea.addEventHandler(MouseEvent.MOUSE_CLICKED, e -> {
            if (e.getButton() != MouseButton.PRIMARY) {
                return;
            }
            mousePressed(e);
        });

        // Obsługa rysowania na panelu
        mainArea.addEventHandler(MouseEvent.MOUSE_PRESSED, e -> {
            if (e.getButton() != MouseButton.PRIMARY) {
                return;
            }
            draw(e, mainArea, ButtonsToggleGroup);
        });

        // Kontener BorderPane
        BorderPane borderPane = new BorderPane();
        borderPane.setCenter(mainArea);
        borderPane.setRight(rightMargin);

        // Scena
        Scene scene = new Scene(borderPane, 1300, 800);
        stage.setTitle("figury");
        stage.setScene(scene);
        stage.show();
    }

    /**
     * Wyświetla nowe okno z informacjami o aplikacji.
     */
    private void showInfoWindow() {
        Stage infoStage = new Stage();
        infoStage.setTitle("Informacje");

        VBox infoVBox = new VBox();
        infoVBox.setPadding(new Insets(20));
        infoVBox.setSpacing(10);
        infoVBox.setAlignment(Pos.TOP_LEFT);

        Label infoLabel = new Label("Figury, rysowanie/edycja figur na ekranie, Maksymilian Kułakowski");
        infoLabel.setFont(new Font("Arial", 16));
        Label label2 = new Label("Instrukcja:\n" +
                "kliknij figurę lewym przyciskiem myszy, aby ja aktywować\n" +
                "aktywna figura może:\n" +
                "- Być przesuwana\n" +
                "- Można zmienić jej rozmiar za pomocą scrolla\n" +
                "- obrócić ją o 90 stopni za pomocą przycisku Obróć\n"+
                "klikniecie prawego przycisku myszy na figurze otworzy dodatkowe okienko,\n" +
                "w tym okienku można:\n" +
                " - zmienić kolor figury\n");
        label2.setFont(new Font("Arial", 14));


        infoVBox.getChildren().addAll(infoLabel, label2);

        Scene infoScene = new Scene(infoVBox, 610, 300);
        infoStage.setScene(infoScene);
        infoStage.show();
    }

    /**
     * Rysuje wybrany kształt na panelu.
     * @param e zdarzenie myszy, które wywołuje tę metodę.
     * @param pane panel, na którym rysowany jest kształt.
     * @param shapeToggleGroup grupa przycisków przełączników, które decydują, jaki kształt jest rysowany.
     */
    private void draw(MouseEvent e, Pane pane, ToggleGroup shapeToggleGroup) {
        if (selectedShape == null) {
            return; //jesli nie wcisniety przycisk, nie rysuj
        }

        Shape newShape = null;

        switch (selectedShape) {
            case "Okrąg":
                newShape = new FXCircle(e.getX(), e.getY(), 30);
                break;
            case "Prostokąt":
                newShape = new FXRectangle(e.getX() - 50, e.getY() - 30, 100, 60);
                break;
            case "Trójkąt":
                newShape = new FXTriangle(e.getX(), e.getY());
                break;
        }

        pane.getChildren().add(newShape);
        setActiveShape(newShape);


        //odcisniecie przycisku
        shapeToggleGroup.getSelectedToggle().setSelected(false);
        selectedShape = null;
    }

    /**
     * Obsługuje kliknięcia na figury.
     * @param e zdarzenie myszy, które wywołuje tę metodę.
     */
    private void mousePressed(MouseEvent e) {
        for (int i = mainArea.getChildren().size() - 1; i >= 0; i--) {
            if (mainArea.getChildren().get(i) instanceof Shape) {
                Shape shape = (Shape) mainArea.getChildren().get(i);
                // Używamy localToScene do przeliczenia współrzędnych kliknięcia względem sceny
                if (shape.localToScene(shape.getBoundsInLocal()).contains(e.getSceneX(), e.getSceneY())) {
                    setActiveShape(shape);
                    return;
                }
            }
        }
        setActiveShape(null); // kliknięcie poza figurami wyłącza aktywność
    }
    /**
     * Główna metoda aplikacji.
     * @param args argumenty linii poleceń.
     */
    public static void main(String[] args) {
        launch();
    }
}
