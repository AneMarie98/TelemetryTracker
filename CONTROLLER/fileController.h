#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QJsonDocument>
#include <QFileDialog>

class FileController : public QObject {
    Q_OBJECT

public:
    explicit FileController(QObject *parent = nullptr);

public slots:
    QString openFile();

    void saveFile(const QString &filePath, const QJsonDocument &existingJsonDoc, const QJsonArray &newData, const QString &newFilePath, const QList<int> &deletedSensorIds);
    bool newDataContainsSensor(const QJsonArray &newData, int identifier);

signals:
    void fileOpened(const QString &filePath);
    void fileSaved(const QString &fileName);
    void saveFileError(const QString& errorMessage);

private:
    QString filePath;

};

#endif // FILECONTROLLER_H
