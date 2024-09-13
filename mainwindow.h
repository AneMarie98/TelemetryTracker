#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../TelemetryTracker/CONTROLLER/fileController.h"
#include "../TelemetryTracker/CONTROLLER/jsonController.h"
#include "../TelemetryTracker/VIEW/sensorWidget.h"

#include "../TelemetryTracker/SENSOR//tirePressureSensor.h"
#include "../TelemetryTracker/SENSOR/fuelFlowSensor.h"
#include "../TelemetryTracker/SENSOR/brakeTemperatureSensor.h"

#include <QMainWindow>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QChartView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Main widgets
    QFrame *toolBarFrame;
    QFrame *sensorPanel;
    QVBoxLayout *sensorLogoLayout;
    QFrame *sensorLogoFrame;
    QScrollArea *sensorPanelScrollArea;
    QFrame *buttonLayout;
    QStatusBar *m_statusBar;
    QChartView *chartView;

    // Contained widgets
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *addSensorButton;
    QScrollBar *sensorPanelScrollBar;
    QPushButton *editSensorButton;
    QPushButton *deleteSensorButton;
    QToolButton *openFileButton;
    QToolButton *saveFileButton;

    // Private methods
    void createToolBar();
    void createSensorPanel();
    void createLogoFrame();
    void createChartPanel();
    void createButtonLayout();
    void createStatusBar();

    QString m_filePath;
    QJsonDocument jsonDoc;
    QJsonDocument m_jsonData;
    JsonControllerVisitor* jsonControllerVisitor;

    QVector<SensorWidget*> sensorWidgets;

    QString m_jsonFilePath;
    JsonController m_jsonController;
    QString m_currentFilePath;

    FileController *fileController;
    JsonController *jsonController;

    QVector<int> deletedSensorIds;

    SensorWidget *selectedSensorWidget;
    SensorWidget *m_sensorWidget;

    QChartView * m_chartView;


private slots:
    void openFileClicked();
    void saveFileClicked();
    void showFileSavedMessage(const QString &filePath);
    void loadJsonFile(const QString& filePath);
    void searchSensor(const QString& searchString);
    void addSensor();
    void addSensorToPanel(const QString& sensorType, int identifier, const QString& timestamp, double value);
    void editSensor();
    void deleteSensor();
    void highlightSelectedWidget(SensorWidget* selectedWidget);

    void displaySensorData(QChart *chart, const QString& selectedSensorType);
    void updateChart(const QString& selectedSensorType);

    QMap<QString, QVector<QPair<QDateTime, double>>> collectSensorData();


};

#endif // MAINWINDOW_H

