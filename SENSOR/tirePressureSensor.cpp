#include "../TelemetryTracker/SENSOR/sensorVisitor.h"
#include "../TelemetryTracker/SENSOR/tirePressureSensor.h"

#include <QJsonObject>

namespace Sensor {

TirePressureSensor::TirePressureSensor(const unsigned int& identifier, const std::string& type,
                               const double& tirePressure, QDateTime& timestamp)
    : AbstractSensor(identifier,type,timestamp), tirePressure(tirePressure) {}

double TirePressureSensor::getTirePressure() const {
    return tirePressure;
}


void TirePressureSensor::accept(SensorVisitor& visitor) const {
    visitor.visit(*this);
}

QJsonObject TirePressureSensor::toJson() const {
    QJsonObject obj;
    obj["identifier"] = static_cast<int>(getIdentifier());
    obj["type"] = QString::fromStdString(getType());
    obj["timestamp"] = getTimestamp().toString(Qt::ISODate);
    obj["Tire Pressure"] = getTirePressure();
    return obj;
}

}

