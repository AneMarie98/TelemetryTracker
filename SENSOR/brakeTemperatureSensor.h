#ifndef BRAKETEMPERATURESENSOR_H
#define BRAKETEMPERATURESENSOR_H


#include "../TelemetryTracker/SENSOR/abstractSensor.h"

#include <QDateTime>

namespace Sensor {

class SensorVisitor;

class BrakeTemperatureSensor: public AbstractSensor {
private:
    double brakeTemperature;
public:
    BrakeTemperatureSensor(const unsigned int& identifier, const std::string&type,
                           const double& brakeTemperature, QDateTime& timestamp);


    double getBrakeTemperature() const;



    void accept(SensorVisitor& visitor) const override;

    QJsonObject toJson() const override;


};
}

#endif // BRAKETEMPERATURESENSOR_H
