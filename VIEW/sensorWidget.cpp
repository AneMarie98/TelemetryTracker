#include "../TelemetryTracker/VIEW/sensorWidget.h"
#include "../TelemetryTracker/mainwindow.h"
#include "../TelemetryTracker/SENSOR/abstractSensor.h"

#include <QMouseEvent>


SensorWidget::SensorWidget(QWidget *parent) : QWidget(parent), m_identifier(0), m_type(""){
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_sensorDataLabel = new QLabel(this);
    layout->addWidget(m_sensorDataLabel);
    setFocusPolicy(Qt::ClickFocus);

}


SensorWidget::~SensorWidget() {}

int SensorWidget::getIdentifier() const {
    return m_identifier;
}

QString SensorWidget::getType() const {
    return m_type;
}

QString SensorWidget::getTimestamp() const {
    return m_timestamp;
}

QString SensorWidget::getValueName() const {
    return m_valueName;
}

double SensorWidget::getNumericData() const {
    return m_numericData;
}

Sensor::AbstractSensor* SensorWidget::getSensor() const {

    return associatedSensor;
}


void SensorWidget::setIdentifier(int identifier) {
    m_identifier = identifier;

}


void SensorWidget::setType(const QString& type) {
    m_type = type;

}

void SensorWidget::setTimestamp(const QString& timestamp) {
    m_timestamp = timestamp;

}

void SensorWidget::setValueName(const QString& valueName) {
    m_valueName = valueName;

}

void SensorWidget::setNumericData(double data) {
    m_numericData = data;
    m_sensorDataLabel->setText("Identifier: " + QString::number(m_identifier) + "\nType: " + m_type + "\nTimestamp: " + m_timestamp + "\n" + m_valueName + ": " + QString::number(m_numericData));
}

void SensorWidget::setStyle(const QString& style) {
    setStyleSheet(style);
}

void SensorWidget::setSensorData(Sensor::AbstractSensor* sensor) {
    associatedSensor = sensor;
    m_identifier = sensor->getIdentifier();
    m_type = QString::fromStdString(sensor->getType());
    m_timestamp = sensor->getTimestamp().toString();

    if (auto brakeSensor = dynamic_cast<Sensor::BrakeTemperatureSensor*>(sensor)) {
        m_valueName = "Brake Temperature";
        setNumericData(brakeSensor->getBrakeTemperature());
    } else if (auto fuelSensor = dynamic_cast<Sensor::FuelFlowSensor*>(sensor)) {
        m_valueName = "Flow Rate";
        setNumericData(fuelSensor->getFlowRate());
    } else if (auto tireSensor = dynamic_cast<Sensor::TirePressureSensor*>(sensor)) {
        m_valueName = "Tire Pressure";
        setNumericData(tireSensor->getTirePressure());
    } else {
        m_valueName = "Value";
        setNumericData(0.0); // Default value for unknown sensors
    }
}

//void SensorWidget::setSensorData(int identifier, const QString& type, const QString& timestamp, const QString& valueName, double data) {
    //m_identifier = identifier;
    //m_type = type;
    //m_timestamp = timestamp;
    //m_valueName = valueName;
    //m_numericData = data;
    //m_sensorDataLabel->setWordWrap(true);
    //setNumericData(data);

//}

void SensorWidget::mousePressEvent(QMouseEvent *event) {
    emit sensorClicked(this);
    QWidget::mousePressEvent(event);
    emit updateChartRequested(m_type);
}




