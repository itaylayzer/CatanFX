package com.itayl.view.Windows;
// package com.itayl.View.Windows;

// import javafx.scene.Scene;
// import javafx.scene.chart.LineChart;
// import javafx.scene.chart.NumberAxis;
// import javafx.scene.chart.XYChart;
// import javafx.scene.control.Label;
// import javafx.scene.layout.VBox;
// import javafx.stage.Stage;
// import java.lang.management.ManagementFactory;
// import java.lang.management.MemoryMXBean;
// import java.text.DecimalFormat;

// import com.itayl.View.Constants;

// public class MemoryMonitor extends Stage {
// Double maxValue;

// public MemoryMonitor() {
// setTitle("Memory Usage Monitor");

// // Create labels to display current memory usage
// Label heapMemoryLabel = new Label();
// Label nonHeapMemoryLabel = new Label();

// // Create a line chart to display memory usage over time
// final NumberAxis xAxis = new NumberAxis();
// final NumberAxis yAxis = new NumberAxis();
// xAxis.setLabel("Time (seconds)");
// yAxis.setLabel("Memory Usage (MB)");
// xAxis.setAutoRanging(false);
// yAxis.setAutoRanging(false);
// yAxis.setLowerBound(0);
// final LineChart<Number, Number> memoryChart = new LineChart<>(xAxis, yAxis);
// memoryChart.setTitle("Memory Usage Over Time");

// // Create series for heap and non-heap memory usage
// final XYChart.Series<Number, Number> heapSeries = new XYChart.Series<>();
// heapSeries.setName("Heap Memory");
// final XYChart.Series<Number, Number> nonHeapSeries = new XYChart.Series<>();
// nonHeapSeries.setName("Non-Heap Memory");

// // Add series to chart
// memoryChart.getData().add(heapSeries);
// memoryChart.getData().add(nonHeapSeries);

// // hide dots
// memoryChart.setCreateSymbols(false);

// VBox root = new VBox(10);
// root.getChildren().addAll(heapMemoryLabel, nonHeapMemoryLabel, memoryChart);

// Scene scene = new Scene(root, 600, 400);
// setScene(scene);

// // Create a DecimalFormat to format memory values
// DecimalFormat format = new DecimalFormat("#.##");

// // Get the MemoryMXBean
// MemoryMXBean memoryMXBean = ManagementFactory.getMemoryMXBean();

// Long startTime = System.currentTimeMillis();

// maxValue = 0.0;
// // Update memory usage labels and chart periodically
// javafx.animation.AnimationTimer timer = new javafx.animation.AnimationTimer()
// {

// @Override
// public void handle(long now) {
// final int fiveSeconds = 5000;
// long timestamp = System.currentTimeMillis() - startTime; // Current time in
// milliseconds

// // Get heap memory usage
// long heapMemoryUsed = memoryMXBean.getHeapMemoryUsage().getUsed();
// long heapMemoryUsedMB = heapMemoryUsed / (1024 * 1024);
// heapMemoryLabel.setText("Heap Memory Used: " +
// format.format(heapMemoryUsedMB) + " MB");

// // Get non-heap memory usage
// long nonHeapMemoryUsed = memoryMXBean.getNonHeapMemoryUsage().getUsed();
// long nonHeapMemoryUsedMB = nonHeapMemoryUsed / (1024 * 1024);
// nonHeapMemoryLabel.setText("Non-Heap Memory Used: " +
// format.format(nonHeapMemoryUsedMB) + " MB");

// maxValue = (double) Math.max(maxValue, Math.max(nonHeapMemoryUsedMB,
// heapMemoryUsedMB));

// // Update series data
// heapSeries.getData().add(new XYChart.Data<>(timestamp, heapMemoryUsedMB));
// nonHeapSeries.getData().add(new XYChart.Data<>(timestamp,
// nonHeapMemoryUsedMB));

// removeOldData(heapSeries, timestamp - fiveSeconds);
// removeOldData(nonHeapSeries, timestamp - fiveSeconds);

// yAxis.setUpperBound(maxValue);
// xAxis.setUpperBound(timestamp);
// if (timestamp - fiveSeconds > 0) {
// xAxis.setLowerBound(timestamp - fiveSeconds);
// }
// }

// private void removeOldData(XYChart.Series<Number, Number> series, double
// threshold) {
// // Iterate through data points and remove those older than the threshold
// series.getData().removeIf(data -> data.getXValue().doubleValue() <
// threshold);
// }
// };
// timer.start();
// getIcons().add(Constants.Images.icon);
// }
// }
