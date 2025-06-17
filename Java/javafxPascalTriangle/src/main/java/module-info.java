module org.example.zad2 {
    requires javafx.controls;
    requires javafx.fxml;


    opens org.example.zad2 to javafx.fxml;
    exports org.example.zad2;
}