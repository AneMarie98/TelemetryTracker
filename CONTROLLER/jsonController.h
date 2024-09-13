#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QVector>
#include "../TelemetryTracker/VIEW/sensorWidget.h"
#include "../TelemetryTracker/SENSOR/abstractSensor.h"


class SensorWidgetStyle {
public:
    void visit(const QJsonObject& jsonObject);

    //int getIdentifier() const { return m_identifier; }
    //QString getType() const { return m_type; }
    //QString getStyle() const {return m_style;}

    Sensor::AbstractSensor* getSensor() const { return m_sensor; }
    SensorWidget* getSensorWidget() const { return m_sensorWidget; }


private:
    //int m_identifier;
    //QString m_type;
    //QString m_style;

    Sensor::AbstractSensor* m_sensor;
    SensorWidget* m_sensorWidget;

};

class JsonControllerVisitor {
public:


    JsonControllerVisitor();

    void visit(const QJsonObject& jsonObject);
    void visit(const QJsonArray& jsonArray);


    QVector<SensorWidget*> getSensorWidgets() const { return m_sensorWidgets; }



private:
    QVector<SensorWidget*> m_sensorWidgets;
};

class JsonController : public QObject {
    Q_OBJECT
public:
    explicit JsonController(QObject* parent = nullptr);
    ~JsonController();


    QJsonArray getTemporaryData() const;
    QVector<Sensor::AbstractSensor*> getSensorData() const { return m_sensorData; }
    void addSensorData(Sensor::AbstractSensor* sensor);
    void setFilePath(const QString &filePath);
    void updateSensorData(Sensor::AbstractSensor* sensor);
    void clearTemporaryData();
    void deleteSensorData(int identifier);


private:
    QString m_filePath;
    QJsonArray m_temporaryData;
    QVector<Sensor::AbstractSensor*> m_sensorData;
};

#endif // JSONCONTROLLER_H

