package org.example.zad;

import javafx.event.EventHandler;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.shape.Rectangle;
import javafx.scene.paint.Color;
import javafx.scene.input.MouseButton;

/**
 * Klasa reprezentująca prostokąt w aplikacji.
 */
public class FXRectangle extends Rectangle {
    /**
     * Konstruktor klasy FXRectangle.
     * @param x współrzędna x prostokąta.
     * @param y współrzędna y prostokąta.
     * @param width szerokość prostokąta.
     * @param height wysokość prostokąta.
     */
    public FXRectangle(double x, double y, double width, double height) {
        super(x, y, width, height);
        setFill(Color.RED);
        setOnMouseClicked(new FXRectangleEventHandler());
        setOnMouseDragged(new FXRectangleEventHandler());
        setOnScroll(new FXRectangleScrollHandler());
    }

    /**
     * Sprawdza, czy punkt o podanych współrzędnych znajduje się wewnątrz prostokąta.
     * @param x współrzędna x punktu.
     * @param y współrzędna y punktu.
     * @return true, jeśli punkt znajduje się wewnątrz prostokąta, false w przeciwnym razie.
     */
    public boolean isHit(double x, double y) {
        return getBoundsInLocal().contains(x, y);
    }

    /**
     * Zmienia współrzędną x prostokąta.
     * @param x wartość, która ma być dodana do współrzędnej x.
     */
    public void addX(double x) {
        setX(getX() + x);
    }

    /**
     * Zmienia współrzędną y prostokąta.
     * @param y wartość, która ma być dodana do współrzędnej y.
     */
    public void addY(double y) {
        setY(getY() + y);
    }

    /**
     * Zmienia szerokość prostokąta.
     * @param w wartość, która ma być dodana do szerokości prostokąta.
     */
    public void addWidth(double w) {
        setWidth(getWidth() + w);
    }

    /**
     * Zmienia wysokość prostokąta.
     * @param h wartość, która ma być dodana do wysokości prostokąta.
     */
    public void addHeight(double h) {
        setHeight(getHeight() + h);
    }

    /**
     * Dodaje obramowanie do prostokąta.
     */
    public void addBorder() {
        setStroke(Color.BLUE);
        setStrokeWidth(3);
    }

    /**
     * Usuwa obramowanie z prostokąta.
     */
    public void removeBorder() {
        setStroke(null);
    }

    /**
     * implementacja przesuwania.
     */
    private class FXRectangleEventHandler implements EventHandler<MouseEvent> {
        FXRectangle rectangle;
        private double x;
        private double y;

        private void doMove(MouseEvent event) {
            rectangle = (FXRectangle) event.getSource();
            double dx = event.getX() - x;
            double dy = event.getY() - y;

            if (HelloApplication.getActiveShape() == rectangle && rectangle.isHit(x, y)) {
                rectangle.addX(dx);
                rectangle.addY(dy);
            }
            x += dx;
            y += dy;
        }

        @Override
        public void handle(MouseEvent event) {
            rectangle = (FXRectangle) event.getSource();
            if (event.getButton() != MouseButton.PRIMARY && event.getButton() != MouseButton.SECONDARY) {
                return;
            }
            if (event.getEventType() == MouseEvent.MOUSE_CLICKED) {
                x = event.getX();
                y = event.getY();
                if (event.getButton() == MouseButton.PRIMARY) {
                    HelloApplication.setActiveShape(rectangle);
                } else if (event.getButton() == MouseButton.SECONDARY && HelloApplication.getActiveShape() == rectangle) {
                    FXColorPicker colorPicker = new FXColorPicker();
                    colorPicker.show(HelloApplication.getStage(), event.getScreenX(), event.getScreenY(), rectangle);
                }
            }
            if (event.getEventType() == MouseEvent.MOUSE_DRAGGED && event.getButton() == MouseButton.PRIMARY) {
                doMove(event);
            }
        }

    }
    /**
     * Klasa obsługująca zdarzenia scrollowania dla prostokąta.
     */
    private class FXRectangleScrollHandler implements EventHandler<ScrollEvent> {
        FXRectangle rectangle;
        private void doScale(ScrollEvent event) {
            rectangle = (FXRectangle) event.getSource();
            double x = event.getX();
            double y = event.getY();

            if (HelloApplication.getActiveShape() == rectangle && isHit(x, y)) {
                rectangle.addWidth(event.getDeltaY() * 0.2);
                rectangle.addHeight(event.getDeltaY() * 0.2);
            }
        }


        @Override
        public void handle(ScrollEvent event) {
            if (event.getEventType() == ScrollEvent.SCROLL) {
                doScale(event);
            }
        }
    }
}
