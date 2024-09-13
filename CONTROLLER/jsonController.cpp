#include "../TelemetryTracker/CONTROLLER/jsonController.h"
#include "../TelemetryTracker/VIEW/sensorWidget.h"
#include "../TelemetryTracker/SENSOR/abstractSensor.h"
#include "../TelemetryTracker/SENSOR/brakeTemperatureSensor.h"
#include "../TelemetryTracker/SENSOR/fuelFlowSensor.h"
#include "../TelemetryTracker/SENSOR/tirePressureSensor.h"

#include <QFile>


void SensorWidgetStyle::visit(const QJsonObject& jsonObject) {
    unsigned int identifier = jsonObject["identifier"].toInt();
    std::string type = jsonObject["type"].toString().toStdString();
    QDateTime timestamp = QDateTime::fromString(jsonObject["timestamp"].toString(), Qt::ISODate);

    if (type == "Tire Pressure") {
        double tirePressure = jsonObject["Tire Pressure"].toDouble();
        m_sensor = new Sensor::TirePressureSensor(identifier, type, tirePressure, timestamp);
    } else if (type == "Brake Temperature") {
        double brakeTemperature = jsonObject["Brake Temperature"].toDouble();
        m_sensor = new Sensor::BrakeTemperatureSensor(identifier, type, brakeTemperature, timestamp);
    } else if (type == "Fuel Flow") {
        double flowRate = jsonObject["Flow Rate"].toDouble();
        m_sensor = new Sensor::FuelFlowSensor(identifier, type, flowRate, timestamp);
    }


    QString style;
    if (type == "Tire Pressure") {
        style = "background-color: #d73c37; color: white;";
    } else if (type == "Brake Temperature") {
        style = "background-color: #f76f6b; color: white;";
    } else if (type == "Fuel Flow") {
        style = "background-color: #a91b16; color: white;";
    } else {
        style = "background-color: gray; color: black;";
    }

    m_sensorWidget = new SensorWidget();
    m_sensorWidget->setSensorData(m_sensor);
    m_sensorWidget->setStyle(style);

}


void JsonControllerVisitor::visit(const QJsonObject& jsonObject) {

    SensorWidgetStyle sensorVisitor;
    sensorVisitor.visit(jsonObject);
    m_sensorWidgets.append(sensorVisitor.getSensorWidget());
}

void JsonControllerVisitor::visit(const QJsonArray& jsonArray) {

    for (const auto& item : jsonArray) {
        QJsonObject jsonObject = item.toObject();
        visit(jsonObject);
    }
}

//void JsonController::addSensorData(int identifier, const QString& type, const QString& timestamp, const QString& valueName, double data) {
    //QJsonObject sensorData;
    //sensorData["identifier"] = identifier;
    //sensorData["type"] = type;
    //sensorData["timestamp"] = timestamp;


    //if (valueName == "Flow Rate" || valueName == "Tire Pressure" || valueName == "Brake Temperature") {
        //sensorData[valueName] = data;
    //} else {

        //sensorData["data"] = data;
    //}

    //m_temporaryData.append(sensorData);
//}

void JsonController::addSensorData(Sensor::AbstractSensor* sensor) {
    m_sensorData.append(sensor);
}


QJsonArray JsonController::getTemporaryData() const {
    return m_temporaryData;
}



void JsonController::setFilePath(const QString &filePath) {
    m_filePath = filePath;
}

//void JsonController::updateSensorData(int identifier, const QString& type, const QString& timestamp, double data, const QString& valueName) {
    // Search for specified identifier
    //for (auto& sensorData : m_sensorData) {
        //if (sensorData["identifier"].toInt() == identifier) {
            // Update data
            //sensorData["type"] = type;
            //sensorData["timestamp"] = timestamp;
            //sensorData[valueName] = data;
            //return;
        //}
    //}

    // If sensor not fount, add sensor
    //QJsonObject newSensorData;
    //newSensorData["identifier"] = identifier;
    //newSensorData["type"] = type;
    //newSensorData["timestamp"] = timestamp;
    //newSensorData[valueName] = data;
    //m_sensorData.append(newSensorData);
//}

void JsonController::updateSensorData(Sensor::AbstractSensor* sensor) {
    for (int i = 0; i < m_sensorData.size(); ++i) {
        if (m_sensorData[i]->getIdentifier() == sensor->getIdentifier()) {
            delete m_sensorData[i];
            m_sensorData[i] = sensor;
            return;
        }
    }
    m_sensorData.append(sensor);
}

void JsonController::clearTemporaryData() {
    qDeleteAll(m_sensorData);
    m_sensorData.clear();
}


void JsonController::deleteSensorData(int identifier) {
    for (int i = 0; i < m_sensorData.size(); ++i) {
        if (m_sensorData[i]->getIdentifier() == identifier) {
            delete m_sensorData[i];
            m_sensorData.removeAt(i);
            return;
        }
    }
}



JsonControllerVisitor::JsonControllerVisitor() {}

JsonController::JsonController(QObject* parent) : QObject(parent) {}

JsonController::~JsonController() {
    clearTemporaryData();
}



