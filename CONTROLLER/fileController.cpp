#include "../TelemetryTracker/CONTROLLER/fileController.h"
#include "../TelemetryTracker/mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

FileController::FileController(QObject *parent) : QObject(parent){}

QString FileController::openFile()
{
    // Opens a dialogue window to select a JSON file
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Open JSON File"), QDir::homePath(), tr("JSON Files (*.json)"));

    emit fileOpened(filePath);

    // Returns JSON file path
    return filePath;
}


void FileController::saveFile(const QString &filePath, const QJsonDocument &existingJsonDoc, const QJsonArray &newData, const QString &newFilePath, const QList<int> &deletedSensorIds) {
    // Combine existing data with new data and modified data
    QJsonArray existingData = existingJsonDoc.array();

    // Copy the list of deleted sensor IDs
    QList<int> deletedIdsCopy = deletedSensorIds;

    // Loop through existing data
    for (int i = 0; i < existingData.size(); ++i) {
        QJsonObject existingSensorData = existingData[i].toObject();
        int existingIdentifier = existingSensorData["identifier"].toInt();

        //Check if the identifier exists in the list of deleted sensor IDs, skip this sensor if found
        if (deletedIdsCopy.contains(existingIdentifier)) {
            existingData.removeAt(i);
            --i; // Adjust index after removal
            continue;
        }
    }

    // Append new data to the existing data
    for (const auto& sensorData : newData) {
        QJsonObject newSensorData = sensorData.toObject();
        int newIdentifier = newSensorData["identifier"].toInt();

        // Check if the new sensor data already exists in the existing data
        bool found = false;
        for (const auto& existingSensorData : existingData) {
            int existingIdentifier = existingSensorData.toObject()["identifier"].toInt();
            if (existingIdentifier == newIdentifier) {
                found = true;
                break;
            }
        }

        // If the new sensor data is not found in the existing data, append it
        if (!found) {
            existingData.append(newSensorData);
        }
    }

    // Creates a new JSON document that contains the updated data
    QJsonDocument combinedJsonDoc(existingData);

    // Creates a QFile object for the new JSON file
    QFile newFile(newFilePath);

    // Tries to open the new file for writing
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << newFile.errorString();
        emit saveFileError("Unable to open file for writing.");
        return;
    }

    // Writes the combined JSON data into a new file
    newFile.write(combinedJsonDoc.toJson());

    // Close the new file
    newFile.close();

    // File saved signal
    emit fileSaved(newFilePath);
}


bool FileController::newDataContainsSensor(const QJsonArray &newData, int identifier) {
    // Loop through new data to check if the identifier exists
    for (const auto& sensorData : newData) {
        QJsonObject newSensorData = sensorData.toObject();
        int newIdentifier = newSensorData["identifier"].toInt();
        if (identifier == newIdentifier) {
            return true;
        }
    }
    return false;
}
