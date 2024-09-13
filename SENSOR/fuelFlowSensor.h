#ifndef FUELFLOWSENSOR_H
#define FUELFLOWSENSOR_H



#include "../TelemetryTracker/SENSOR/abstractSensor.h"

#include <string>
#include <QDateTime>

namespace Sensor {

class SensorVisitor;

class FuelFlowSensor: public AbstractSensor {

private:
    double flowRate;
public:
    FuelFlowSensor(const unsigned int& identifier, const std::string&type,
                           const double& flowRate, QDateTime& timestamp);


    double getFlowRate() const;



    void accept(SensorVisitor& visitor) const override;

    QJsonObject toJson() const override;


};
}

#endif // FUELFLOWSENSOR_H
