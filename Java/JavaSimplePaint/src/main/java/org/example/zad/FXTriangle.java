package org.example.zad;

import javafx.event.EventHandler;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Polygon;

/**
 * Klasa reprezentująca trójkąt w aplikacji.
 */
public class FXTriangle extends Polygon {
    /**
     * Konstruktor klasy FXTriangle.
     * @param x współrzędna x środka trójkąta.
     * @param y współrzędna y środka trójkąta.
     */
    public FXTriangle(double x, double y) {
        super(
                x, y - 30,
                x - 30, y + 30,
                x + 30, y + 30
        );
        setFill(Color.GREEN);
        setOnMouseClicked(new FXTriangleEventHandler());
        setOnMouseDragged(new FXTriangleEventHandler());
        setOnScroll(new FXTriangleScrollHandler());
    }
    /**
     * Sprawdza, czy punkt o podanych współrzędnych znajduje się wewnątrz trójkąta.
     * @param x współrzędna x punktu.
     * @param y współrzędna y punktu.
     * @return true, jeśli punkt znajduje się wewnątrz trójkąta, false w przeciwnym razie.
     */
    public boolean isHit(double x, double y) {
        return contains(x, y);
    }
    /**
     * Zmienia współrzędną x środka trójkąta.
     * @param x wartość, która ma być dodana do współrzędnej x środka trójkąta.
     */
    public void addX(double x) {
        for (int i = 0; i < getPoints().size(); i += 2) {
            getPoints().set(i, getPoints().get(i) + x);
        }
    }
    /**
     * Zmienia współrzędną y środka trójkąta.
     * @param y wartość, która ma być dodana do współrzędnej y środka trójkąta.
     */
    public void addY(double y) {
        for (int i = 1; i < getPoints().size(); i += 2) {
            getPoints().set(i, getPoints().get(i) + y);
        }
    }
    /**
     * Zmienia szerokość trójkąta.
     * @param w wartość, która ma być dodana do szerokości trójkąta.
     */
    public void addWidth(double w) {
        double centerX = (getPoints().get(0) + getPoints().get(2) + getPoints().get(4)) / 3;
        for (int i = 0; i < getPoints().size(); i += 2) {
            double currentX = getPoints().get(i);
            double newX = centerX + (currentX - centerX) * (1 + w / 100);
            getPoints().set(i, newX);
        }
    }
    /**
     * Zmienia wysokość trójkąta.
     * @param h wartość, która ma być dodana do wysokości trójkąta.
     */
    public void addHeight(double h) {
        double centerY = (getPoints().get(1) + getPoints().get(3) + getPoints().get(5)) / 3;
        for (int i = 1; i < getPoints().size(); i += 2) {
            double currentY = getPoints().get(i);
            double newY = centerY + (currentY - centerY) * (1 + h / 100);
            getPoints().set(i, newY);
        }
    }
    /**
     * Dodaje obramowanie do trójkąta.
     */
    public void addBorder() {
        setStroke(Color.BLUE);
        setStrokeWidth(3);
    }
    /**
     * Usuwa obramowanie z trójkąta.
     */
    public void removeBorder() {
        setStroke(null);
    }
    /**
     * Klasa obsługująca zdarzenia myszy dla trójkąta.
     */
    private class FXTriangleEventHandler implements EventHandler<MouseEvent> {
        FXTriangle triangle;
        private double x;
        private double y;

        private void doMove(MouseEvent event) {
            triangle = (FXTriangle) event.getSource();
            double dx = event.getX() - x;
            double dy = event.getY() - y;

            if (HelloApplication.getActiveShape() == triangle && isHit(x, y)) {
                triangle.addX(dx);
                triangle.addY(dy);
            }
            x += dx;
            y += dy;
        }

        @Override
        public void handle(MouseEvent event) {
            triangle = (FXTriangle) event.getSource();
            if (event.getButton() != MouseButton.PRIMARY && event.getButton() != MouseButton.SECONDARY) {
                return;
            }
            if (event.getEventType() == MouseEvent.MOUSE_CLICKED) {
                x = event.getX();
                y = event.getY();
                if (event.getButton() == MouseButton.PRIMARY) {
                    HelloApplication.setActiveShape(triangle);
                } else if (event.getButton() == MouseButton.SECONDARY && HelloApplication.getActiveShape() == triangle) {
                    FXColorPicker colorPicker = new FXColorPicker();
                    colorPicker.show(HelloApplication.getStage(), event.getScreenX(), event.getScreenY(), triangle);
                }
                event.consume();
            }
            if (event.getEventType() == MouseEvent.MOUSE_DRAGGED && event.getButton() == MouseButton.PRIMARY) {
                doMove(event);
            }
        }
    }
    /**
     * Klasa obsługująca zdarzenia scrollowania dla trójkąta.
     */
    private class FXTriangleScrollHandler implements EventHandler<ScrollEvent> {
        FXTriangle triangle;
        private void doScale(ScrollEvent event) {
            triangle = (FXTriangle) event.getSource();
            double x = event.getX();
            double y = event.getY();

            if (HelloApplication.getActiveShape() == triangle && isHit(x, y)) {
                triangle.addWidth(event.getDeltaY() * 0.2);
                triangle.addHeight(event.getDeltaY() * 0.2);
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
