#include "../TelemetryTracker/SENSOR/sensorVisitor.h"
#include "../TelemetryTracker/SENSOR/brakeTemperatureSensor.h"

#include <QJsonObject>


namespace Sensor {

BrakeTemperatureSensor::BrakeTemperatureSensor(const unsigned int& identifier, const std::string& type,
                                                const double& brakeTemperature, QDateTime& timestamp)
    : AbstractSensor(identifier,type,timestamp), brakeTemperature(brakeTemperature) {}

double BrakeTemperatureSensor::getBrakeTemperature() const {
    return brakeTemperature;
}



void BrakeTemperatureSensor::accept(SensorVisitor& visitor) const {
    visitor.visit(*this);
}

QJsonObject BrakeTemperatureSensor::toJson() const {
    QJsonObject obj;
    obj["identifier"] = static_cast<int>(getIdentifier());
    obj["type"] = QString::fromStdString(getType());
    obj["timestamp"] = getTimestamp().toString(Qt::ISODate);
    obj["Brake Temperature"] = getBrakeTemperature();
    return obj;
}

}
