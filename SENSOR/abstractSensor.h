#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H

#include <string>
#include <QDateTime>


namespace Sensor {

class SensorVisitor;

class AbstractSensor {
private:

    const unsigned int identifier;
    std::string type;
    QDateTime timestamp;


protected:
    AbstractSensor(const unsigned int& identifier, const std::string& type,QDateTime& timestamp);

public:

    const unsigned int& getIdentifier() const;
    std::string getType() const;
    QDateTime getTimestamp() const;
    void setType(const std::string& type);



    virtual void accept(SensorVisitor& visitor) const = 0;

    virtual QJsonObject toJson() const = 0;

    virtual ~AbstractSensor() = default;
};

} // namespace Sensor

#endif // ABSTRACTSENSOR_H
