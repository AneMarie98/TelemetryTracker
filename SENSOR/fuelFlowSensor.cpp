#include "../TelemetryTracker/SENSOR/sensorVisitor.h"
#include "../TelemetryTracker/SENSOR/fuelFlowSensor.h"

#include <QJsonObject>

namespace Sensor {

FuelFlowSensor::FuelFlowSensor(const unsigned int& identifier, const std::string& type,
                                               const double& flowRate, QDateTime& timestamp)
    : AbstractSensor(identifier,type,timestamp), flowRate(flowRate) {}

double FuelFlowSensor::getFlowRate() const {
    return flowRate;
}



void FuelFlowSensor::accept(SensorVisitor& visitor) const {
    visitor.visit(*this);
}

QJsonObject FuelFlowSensor::toJson() const {
    QJsonObject obj;
    obj["identifier"] = static_cast<int>(getIdentifier());
    obj["type"] = QString::fromStdString(getType());
    obj["timestamp"] = getTimestamp().toString(Qt::ISODate);
    obj["Flow Rate"] = getFlowRate();
    return obj;
}

}
