#ifndef SENSORVISITOR_H
#define SENSORVISITOR_H

#include "../TelemetryTracker/SENSOR/abstractSensor.h"
#include "../TelemetryTracker/SENSOR/brakeTemperatureSensor.h"
#include "../TelemetryTracker/SENSOR/fuelFlowSensor.h"
#include "../TelemetryTracker/SENSOR/tirePressureSensor.h"

#include <QVector>
#include <QMap>

namespace Sensor {

class SensorVisitor {
public:
    virtual void visit(const AbstractSensor& sensor) = 0;

    virtual ~SensorVisitor() = default;
};

class SensorDataVisitor : public SensorVisitor {
private:
    QMap<QString, QVector<QPair<QDateTime, double>>> sensorData;

public:
    void visit(const AbstractSensor& sensor) override {
        // Add sensor data to sensorData
        const QString& sensorType = QString::fromStdString(sensor.getType());
        QDateTime timestamp = sensor.getTimestamp();
        double value = 0.0;
        // Add value
        if (const TirePressureSensor* tirePressureSensor = dynamic_cast<const TirePressureSensor*>(&sensor)) {
            value = tirePressureSensor->getTirePressure();
        } else if (const BrakeTemperatureSensor* brakeTemperatureSensor = dynamic_cast<const BrakeTemperatureSensor*>(&sensor)) {
            value = brakeTemperatureSensor->getBrakeTemperature();
        } else if (const FuelFlowSensor* fuelFlowSensor = dynamic_cast<const FuelFlowSensor*>(&sensor)) {
            value = fuelFlowSensor->getFlowRate();
        }
        sensorData[sensorType].append(qMakePair(timestamp, value));
    }

    const QMap<QString, QVector<QPair<QDateTime, double>>>& getSensorData() const {
        return sensorData;
    }
};

} // namespace Sensor

#endif // SENSORVISITOR_H
