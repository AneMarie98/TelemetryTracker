#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H


#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "../TelemetryTracker/SENSOR/abstractSensor.h"



class SensorWidget : public QWidget {
    Q_OBJECT


public:
    explicit SensorWidget(QWidget *parent = nullptr);
    ~SensorWidget();

    int getIdentifier() const;
    QString getType() const;
    QString getTimestamp() const;
    QString getValueName() const;
    double getNumericData() const;

    Sensor::AbstractSensor* getSensor() const;

    void setIdentifier(int identifier);
    void setType(const QString& type);

    void setTimestamp(const QString& timestamp);
    void setValueName(const QString& valueName);
    void setNumericData(double data);

    void setSensorData(Sensor::AbstractSensor* sensor);

    void setStyle(const QString& style);

signals:
    void clicked();
    void sensorClicked(SensorWidget *widget);
    void updateChartRequested(const QString& sensorType);

private:

    int m_identifier;
    QString m_type;
    QString m_timestamp;
    QString m_valueName;
    double m_numericData;

    Sensor::AbstractSensor* associatedSensor;

    QLabel* m_identifierLabel;
    QLabel* m_typeLabel;
    QLabel* m_timestampLabel;
    QLabel* m_valueNameLabel;
    QLabel* m_numericDataLabel;
    QLabel* m_sensorDataLabel;

protected:

    void mousePressEvent(QMouseEvent *event) override;


};

#endif // SENSORWIDGET_H
