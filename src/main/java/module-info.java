module com.c1t45 {
    requires javafx.controls;
    requires javafx.fxml;

    opens com.c1t45 to javafx.fxml;
    opens com.c1t45.controller to javafx.fxml;
    exports com.c1t45;
}