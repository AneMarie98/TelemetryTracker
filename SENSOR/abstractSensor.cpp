#include "../TelemetryTracker/SENSOR/sensorVisitor.h"
#include "../TelemetryTracker/SENSOR/abstractSensor.h"
#include "../TelemetryTracker/SENSOR/brakeTemperatureSensor.h"
#include "../TelemetryTracker/SENSOR/fuelFlowSensor.h"
#include "../TelemetryTracker/SENSOR/tirePressureSensor.h"

namespace Sensor{

AbstractSensor::AbstractSensor(const unsigned int& identifier, const std::string& type,QDateTime& timestamp) : identifier(identifier), type(type) , timestamp(timestamp) {}


const unsigned int& AbstractSensor::getIdentifier() const {
    return identifier;
}

std::string AbstractSensor::getType() const {
    return type;
}

QDateTime AbstractSensor::getTimestamp() const {
    return timestamp;
}

void AbstractSensor::setType(const std::string& type) {
    this->type = type;
}

}

