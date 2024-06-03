module com.itayl {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.base;
    requires javafx.graphics;
    requires java.desktop;

    opens com.itayl to javafx.fxml;
    opens com.itayl.controller to javafx.fxml;

    exports com.itayl;
}
