#ifndef TIREPRESSURESENSOR_H
#define TIREPRESSURESENSOR_H



#include "../TelemetryTracker/SENSOR/abstractSensor.h"


#include <QDateTime>

namespace Sensor {

class SensorVisitor;

class TirePressureSensor: public AbstractSensor {
private:
    double tirePressure;
public:
    TirePressureSensor(const unsigned int& identifier, const std::string&type,
                           const double& tirePressure, QDateTime& timestamp);


    double getTirePressure() const;



    void accept(SensorVisitor& visitor) const override;

    QJsonObject toJson() const override;


};
}


#endif // TIREPRESSURESENSOR_H
