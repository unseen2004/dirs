package org.example.zad;

import javafx.event.EventHandler;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

/**
 * Klasa reprezentująca okrąg w aplikacji.
 */
public class FXCircle extends Circle {
    /**
     * Konstruktor klasy FXCircle.
     * @param x współrzędna x środka okręgu.
     * @param y współrzędna y środka okręgu.
     * @param radius promień okręgu.
     */
    public FXCircle(double x, double y, double radius) {
        super(x, y, radius);
        setFill(Color.PERU);
        setOnMouseClicked(new FXCircleEventHandler());
        setOnMouseDragged(new FXCircleEventHandler());
        setOnScroll(new FXCircleScrollHandler());
    }
    /**
     * Sprawdza, czy punkt o podanych współrzędnych znajduje się wewnątrz okręgu.
     * @param x współrzędna x punktu.
     * @param y współrzędna y punktu.
     * @return true, jeśli punkt znajduje się wewnątrz okręgu, false w przeciwnym razie.
     */
    public boolean isHit(double x, double y) {
        return getBoundsInLocal().contains(x, y);
    }
    /**
     * Zmienia współrzędną x środka okręgu.
     * @param x wartość, która ma być dodana do współrzędnej x środka okręgu.
     */
    public void addX(double x) {
        setCenterX(getCenterX() + x);
    }
    /**
     * Zmienia współrzędną y środka okręgu.
     * @param y wartość, która ma być dodana do współrzędnej y środka okręgu.
     */
    public void addY(double y) {
        setCenterY(getCenterY() + y);
    }

    public void addWidth(double w) {
        setRadius(getRadius() + w);
    }

    public void addHeight(double h) {
        setRadius(getRadius() + h);
    }
    /**
     * Dodaje obramowanie do okręgu.
     */
    public void addBorder() {
        setStroke(Color.BLUE);
        setStrokeWidth(3);
    }
    /**
     * Usuwa obramowanie z okręgu.
     */
    public void removeBorder() {
        setStroke(null);
    }
    /**
     * Klasa obsługująca zdarzenia myszy dla okręgu.
     */
    private class FXCircleEventHandler implements EventHandler<MouseEvent> {
        FXCircle circle;
        private double x;
        private double y;

        private void doMove(MouseEvent event) {
            circle = (FXCircle) event.getSource();
            double dx = event.getX() - x;
            double dy = event.getY() - y;

            if (HelloApplication.getActiveShape() == circle && isHit(x, y)) {
                circle.addX(dx);
                circle.addY(dy);
            }
            x += dx;
            y += dy;
        }

        @Override
        public void handle(MouseEvent event) {
            circle = (FXCircle) event.getSource();
            if (event.getButton() != MouseButton.PRIMARY && event.getButton() != MouseButton.SECONDARY) {
                return;
            }
            if (event.getEventType() == MouseEvent.MOUSE_CLICKED) {
                x = event.getX();
                y = event.getY();
                if (event.getButton() == MouseButton.PRIMARY) {
                    HelloApplication.setActiveShape(circle);
                } else if (event.getButton() == MouseButton.SECONDARY && HelloApplication.getActiveShape() == circle) {
                    FXColorPicker colorPicker = new FXColorPicker();
                    colorPicker.show(HelloApplication.getStage(), event.getScreenX(), event.getScreenY(), circle);
                }

            }
            if (event.getEventType() == MouseEvent.MOUSE_DRAGGED && event.getButton() == MouseButton.PRIMARY) {
                doMove(event);
            }
        }

    }
    /**
     * Klasa obsługująca zdarzenia scrollowania dla okręgu.
     */
    private class FXCircleScrollHandler implements EventHandler<ScrollEvent> {
        FXCircle circle;
        private void doScale(ScrollEvent event) {
            circle = (FXCircle) event.getSource();
            double x = event.getX();
            double y = event.getY();

            if (HelloApplication.getActiveShape() == circle && isHit(x, y)) {
                circle.addWidth(event.getDeltaY() * 0.2);
                circle.addHeight(event.getDeltaY() * 0.2);
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
