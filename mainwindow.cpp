#include "../TelemetryTracker//mainwindow.h"
#include "../TelemetryTracker/CONTROLLER/fileController.h"
#include "../TelemetryTracker/CONTROLLER/jsonController.h"
#include "../TelemetryTracker/VIEW/sensorWidget.h"
#include "../TelemetryTracker/SENSOR/sensorVisitor.h"




#include <QFile>
#include <QJsonDocument>
#include <QRegularExpression>

#include <QComboBox>
#include <QMessageBox>
#include <QRegularExpression>

#include <QChartView>
#include <QLineSeries>
#include <QLegendMarker>
#include <QDateTimeAxis>
#include <QDateTimeEdit>
#include <QValueAxis>

#include <QScatterSeries>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    createToolBar();
    createSensorPanel();
    createLogoFrame();
    createChartPanel();
    createButtonLayout();
    createStatusBar();

    // Set Mainwindow title
    setWindowTitle("Telemetry Tracker");

    resize(1000, 650);

    // Tool button OpenFileButton connection to controller
    connect(openFileButton, &QToolButton::clicked, this, &MainWindow::openFileClicked);

    // Tool button SaveFileButton connection to controller
    connect(saveFileButton, &QToolButton::clicked, this, &MainWindow::saveFileClicked);

    fileController = new FileController(this);

    connect(fileController, &FileController::fileSaved, this, &MainWindow::showFileSavedMessage);

    jsonController = new JsonController(this);

    connect(fileController, &FileController::fileOpened, jsonController, &JsonController::setFilePath);

    connect(editSensorButton, &QPushButton::clicked, this, &MainWindow::editSensor);

    connect(deleteSensorButton, &QPushButton::clicked, this, &MainWindow::deleteSensor);


}

MainWindow::~MainWindow() {}

void MainWindow::createStatusBar()
{
    // StatusBar
    m_statusBar = new QStatusBar(this);


    m_statusBar->showMessage("Ready", 2000); // Show the message for 2 seconds

    // Set the statusBar to the main window
    setStatusBar(m_statusBar);
}

void MainWindow::createToolBar() {
    //Toolbar

    toolBarFrame = new QFrame(this);
    toolBarFrame->setGeometry(10, 10, 961, 51);
    toolBarFrame->setFrameShape(QFrame::StyledPanel);
    toolBarFrame->setFrameShadow(QFrame::Raised);

    openFileButton = new QToolButton(toolBarFrame);
    openFileButton->setGeometry(20, 10, 51, 22);
    openFileButton->setText("Open");

    saveFileButton = new QToolButton(toolBarFrame);
    saveFileButton->setGeometry(110, 10, 51, 22);
    saveFileButton->setText("Save");

}

void MainWindow::createSensorPanel() {
    sensorPanel = new QFrame(this);
    sensorPanel->setGeometry(30, 80, 281, 491);
    sensorPanel->setFrameShape(QFrame::StyledPanel);
    sensorPanel->setFrameShadow(QFrame::Raised);

    searchLineEdit = new QLineEdit(sensorPanel);
    searchLineEdit->setGeometry(20, 20, 232, 31);


    //Search connection to the changing text within the edit line
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchSensor);


    addSensorButton = new QPushButton(sensorPanel);
    addSensorButton->setGeometry(90, 450, 100, 32);
    addSensorButton->setText("Add Sensor");

    //Add sensor connection to the add sensor button

    connect(addSensorButton, &QPushButton::clicked, this, &MainWindow::addSensor);

    // sensor panel QScrollArea
    sensorPanelScrollArea = new QScrollArea(sensorPanel);
    sensorPanelScrollArea->setGeometry(20, 70, 240, 351);
    sensorPanelScrollArea->setWidgetResizable(true);

    // Contained widgets inside QScrollArea
    QWidget *scrollAreaWidgetContents = new QWidget(sensorPanelScrollArea);
    sensorPanelScrollArea->setWidget(scrollAreaWidgetContents);

    // Layout for contained widgets
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollAreaWidgetContents);
    scrollAreaWidgetContents->setLayout(scrollLayout);

}



void MainWindow::createLogoFrame() {
    sensorLogoFrame = new QFrame(this);
    sensorLogoFrame->setGeometry(330, 80, 641, 101);
    sensorLogoFrame->setFrameShape(QFrame::StyledPanel);
    sensorLogoFrame->setFrameShadow(QFrame::Raised);

    // Logo layout
    sensorLogoLayout = new QVBoxLayout(sensorLogoFrame);
    sensorLogoFrame->setLayout(sensorLogoLayout);

    // QLabel for logo visualization
    QLabel *logoLabel = new QLabel(sensorLogoFrame);

    // Loading logo image
    QPixmap logoPixmap(":/IMAGES/telemetryTrackerLogo");

    // Setting logo width and height
    int newWidth = logoPixmap.width() + 20;

    QPixmap resizedPixmap = logoPixmap.scaledToWidth(newWidth, Qt::SmoothTransformation);

    logoLabel->setPixmap(resizedPixmap);

    // Scaled logo
    logoLabel->setScaledContents(true);

    // Frame dimensions
    int frameWidth = sensorLogoFrame->width();
    int frameHeight = sensorLogoFrame->height();

    // logo image dimensions
    int imageWidth = resizedPixmap.width();
    int imageHeight = resizedPixmap.height();

    // Calculates the position so that the logo is centred
    int x = (frameWidth - imageWidth) / 2;
    int y = (frameHeight - imageHeight) / 2;


    logoLabel->setGeometry(x, y, imageWidth, imageHeight);

    // add QLabel to layout
    sensorLogoLayout->addWidget(logoLabel);


}


void MainWindow::createChartPanel() {
    // Chart creation
    QWidget *chartWidget = new QWidget(this);
    chartWidget->setGeometry(339, 209, 621, 301);
    chartWidget->setStyleSheet("background-color: #F1C5BC;");

    // Create chart
    QChart *chart = new QChart();

    m_chartView = new QChartView(chart, chartWidget);
    m_chartView->setGeometry(chartWidget->rect());

    chartWidget->show();

    // Initially, no sensor selected, so display nothing
    displaySensorData(chart, "");
}


void MainWindow::createButtonLayout() {
    buttonLayout = new QFrame(this);
    buttonLayout->setGeometry(330, 540, 641, 51);
    buttonLayout->setFrameShape(QFrame::StyledPanel);
    buttonLayout->setFrameShadow(QFrame::Raised);

    editSensorButton = new QPushButton(buttonLayout);
    editSensorButton->setGeometry(170, 10, 100, 32);
    editSensorButton->setText("Edit Sensor");

    deleteSensorButton = new QPushButton(buttonLayout);
    deleteSensorButton->setGeometry(318, 10, 100, 32);
    deleteSensorButton->setText("Delete Sensor");

}

void MainWindow::openFileClicked()
{
    // Call openFile method from FileController to get JSON file path
    QString filePath = fileController->openFile();


    if (!filePath.isEmpty()) {
        m_filePath = filePath;


        loadJsonFile(filePath);
    }
}

//void MainWindow::saveFileClicked() {
    //if (m_filePath.isEmpty()) {
        //qWarning() << "No file previously selected.";
        //return;
    //}

   // QString newFilePath = QFileDialog::getSaveFileName(this, tr("Save JSON File"), "", tr("JSON Files (*.json)"));

    //if (!newFilePath.isEmpty()) {
        //QFile existingFile(m_filePath);
       // QJsonDocument existingJsonDoc;

        //if (existingFile.open(QIODevice::ReadOnly)) {
            //QByteArray existingJsonData = existingFile.readAll();
            //existingJsonDoc = QJsonDocument::fromJson(existingJsonData);
            //existingFile.close();

            // Get updated data from json
            //QVector<QJsonObject> updatedSensorData = m_jsonController.getSensorData();

            // Add temporary sensor data
            //QJsonArray temporarySensorData = m_jsonController.getTemporaryData();
            //for (const auto& sensorData : temporarySensorData) {
                //updatedSensorData.append(sensorData.toObject());
            //}

            // Converts QVector<QJsonObject> to QJsonArray
            //QJsonArray updatedData;
            //for (const auto& sensorData : updatedSensorData) {
                //updatedData.append(sensorData);
            //}

            // Save combined data to a new json file
            //fileController->saveFile(m_filePath, existingJsonDoc, updatedData, newFilePath, deletedSensorIds);
        //} else {
            //qWarning() << "Could not open existing file for reading:" << existingFile.errorString();
            //return;
        //}
    //}
//}

void MainWindow::saveFileClicked() {
    if (m_filePath.isEmpty()) {
        qWarning() << "No file previously selected.";
        return;
    }

    QString newFilePath = QFileDialog::getSaveFileName(this, tr("Save JSON File"), "", tr("JSON Files (*.json)"));

    if (!newFilePath.isEmpty()) {
        QFile existingFile(m_filePath);
        QJsonDocument existingJsonDoc;

        if (existingFile.open(QIODevice::ReadOnly)) {
            QByteArray existingJsonData = existingFile.readAll();
            existingJsonDoc = QJsonDocument::fromJson(existingJsonData);
            existingFile.close();

            // Get updated data from json
            QVector<Sensor::AbstractSensor*> updatedSensors = m_jsonController.getSensorData();

            // Convert Sensor::AbstractSensor* to QJsonObject
            QVector<QJsonObject> updatedSensorData;
            for (auto sensor : updatedSensors) {
                updatedSensorData.append(sensor->toJson());
            }

            // Add temporary sensor data
            QJsonArray temporarySensorData = m_jsonController.getTemporaryData();
            for (const auto& sensorData : temporarySensorData) {
                updatedSensorData.append(sensorData.toObject());
            }

            // Convert QVector<QJsonObject> to QJsonArray
            QJsonArray updatedData;
            for (const auto& sensorData : updatedSensorData) {
                updatedData.append(sensorData);
            }

            // Save combined data to a new json file
            fileController->saveFile(m_filePath, existingJsonDoc, updatedData, newFilePath, deletedSensorIds);
        } else {
            qWarning() << "Could not open existing file for reading:" << existingFile.errorString();
            return;
        }
    }
}




void MainWindow::showFileSavedMessage(const QString &filePath) {

    // Confirmation message
    statusBar()->showMessage(tr("File saved successfully!"), 5000);

}


void MainWindow::loadJsonFile(const QString &filePath) {
    // Create QFile object for JSON file
    QFile file(filePath);

    // Open file for reading only
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open JSON file for reading:" << file.errorString();
        return;
    }

    //Read all the data from JSON file
    QByteArray jsonData = file.readAll();

    // close file
    file.close();

    // Converts JSON data into QJsonDocument object
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    // Errors ocurring while parsing
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << parseError.errorString();
        return;
    }

    // Gets widget scroll area layout
    QVBoxLayout *scrollLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());


    if (!scrollLayout) {
        qWarning() << "Could not access scroll area layout.";
        return;
    }

    // Removes all the previous widgets from layout
    QLayoutItem *child;
    while ((child = scrollLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Create new JsonController and JsonControllerVisitor
    JsonController jsonController;
    JsonControllerVisitor visitor;

    // parsing of  JSON document
    if (jsonDoc.isArray()) {
        visitor.visit(jsonDoc.array());
    } else if (jsonDoc.isObject()) {
        visitor.visit(jsonDoc.object());
    } else {
        qWarning() << "JSON data is neither object nor array.";
        return;
    }

    // Gets the sensorWidgets from visitor
    QVector<SensorWidget*> sensorWidgets = visitor.getSensorWidgets();

    QSize sensorWidgetSize(210, 150);

    // Sets a widget dimension
    for (SensorWidget* sensorWidget : sensorWidgets) {


        sensorWidget->setFixedSize(sensorWidgetSize);
        scrollLayout->addWidget(sensorWidget);

        connect(sensorWidget, &SensorWidget::sensorClicked, this, &MainWindow::highlightSelectedWidget);
        connect(sensorWidget, &SensorWidget::updateChartRequested, this, &MainWindow::updateChart);


    }

    // Memorizes JSON document loaded into variable m_jsonData
    m_jsonData = jsonDoc;
}

void MainWindow::searchSensor(const QString& searchString) {
    // Cleans layout
    QLayout* scrollLayout = sensorPanelScrollArea->widget()->layout();

    if (!scrollLayout) {
        qWarning() << "Could not access scroll area layout.";
        return;
    }

    int resultCount = 0; // Counter for search results

    //show all the sensors if search string is empty
    if (searchString.isEmpty()) {
        for (int i = 0; i < scrollLayout->count(); ++i) {
            SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(scrollLayout->itemAt(i)->widget());
            if (sensorWidget) {
                sensorWidget->show();
                resultCount++;
            }
        }
    } else {
        // Check every sensor in the layout
        for (int i = 0; i < scrollLayout->count(); ++i) {
            SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(scrollLayout->itemAt(i)->widget());
            if (!sensorWidget) {
                continue;
            }

            // Gets identifier and sensor type
            int identifier = sensorWidget->getIdentifier();
            QString type = sensorWidget->getType();

            // Verifies if identifier or sensor type contain search string partially
            if (QString::number(identifier).contains(searchString, Qt::CaseInsensitive) ||
                type.contains(searchString, Qt::CaseInsensitive)) {
                // show sensor
                sensorWidget->show();
                resultCount++;
            } else {
                // Hide sensor if it doesn't match the search string
                sensorWidget->hide();
            }
        }
    }
    // Updates the status bar with the search results
    QString statusBarText;
    if (resultCount == 0) {
        statusBarText = "No search results";
    } else if (resultCount == 1) {
        statusBarText = "There is 1 search result";
    } else {
        statusBarText = QString("There are %1 search results").arg(resultCount);
    }

    //Show message
    statusBar()->showMessage(statusBarText);
}


void MainWindow::addSensor() {
    // dialog window for adding a sensor
    QDialog* dialog = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QLabel* label = new QLabel("Select sensor type:", dialog);
    QComboBox* comboBox = new QComboBox(dialog);
    comboBox->addItem("Fuel Flow");
    comboBox->addItem("Tire Pressure");
    comboBox->addItem("Brake Temperature");
    layout->addWidget(label);
    layout->addWidget(comboBox);

    // value label
    QLabel* valueLabel = new QLabel(dialog);
    layout->addWidget(valueLabel);

    // Connection of currentIndexChanged signal to the updating label value function
    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        QString sensorType = comboBox->currentText();
        QString valueLabelText;
        if (sensorType == "Fuel Flow") {
            valueLabelText = "Flow Rate:";
        } else if (sensorType == "Tire Pressure") {
            valueLabelText = "Tire Pressure:";
        } else if (sensorType == "Brake Temperature") {
            valueLabelText = " Brake Temperature:";
        }
        valueLabel->setText(valueLabelText);
    });

    // Data line edit
    QLineEdit* identifierLineEdit = new QLineEdit(dialog);
    QLineEdit* timestampLineEdit = new QLineEdit(dialog);
    QLineEdit* valueLineEdit = new QLineEdit(dialog);
    QLabel* identifierLabel = new QLabel("Identifier:", dialog);
    QLabel* timestampLabel = new QLabel("Timestamp (YYYY-MM-DD HH:MM:SS):", dialog);
    layout->addWidget(identifierLabel);
    layout->addWidget(identifierLineEdit);
    layout->addWidget(timestampLabel);
    layout->addWidget(timestampLineEdit);
    layout->addWidget(valueLabel);
    layout->addWidget(valueLineEdit);

    // Ok button
    QPushButton* okButton = new QPushButton("OK", dialog);
    layout->addWidget(okButton);

    // Connection of OK button to the add sensor logic
    connect(okButton, &QPushButton::clicked, dialog, [this, dialog, comboBox, identifierLineEdit, timestampLineEdit, valueLineEdit]() {
        QString sensorType = comboBox->currentText();
        int identifier = identifierLineEdit->text().toInt();
        QString timestamp = timestampLineEdit->text();
        double value = valueLineEdit->text().toDouble();

        // Data controls
        if (QString::number(identifier).length() != 6) {
            QMessageBox::critical(dialog, "Error", "Identifier must be a 6 digits integer.");
            return;
        }

        QRegularExpression timestampRegex("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
        if (!timestampRegex.match(timestamp).hasMatch()) {
            QMessageBox::critical(dialog, "Error", "Timestamp format not accepted. Timestamp must be YYYY-MM-DD HH:MM:SS.");
            return;
        }

        // Sensor added to the sensor panel
        addSensorToPanel(sensorType, identifier, timestamp, value);


        dialog->close();
    });

    dialog->exec();
}

//void MainWindow::addSensorToPanel(const QString& sensorType, int identifier, const QString& timestamp, double value) {
    // Creates a new sensor widget with fixed size
    //SensorWidget* sensorWidget = new SensorWidget();
    //sensorWidget->setFixedSize(210, 150);

    // Sets sensor data
    //QString valueName;
    //if (sensorType == "Fuel Flow") {
        //sensorWidget->setSensorData(identifier, sensorType, timestamp, "Flow Rate", value);
        //valueName = "Flow Rate";
    //} else if (sensorType == "Tire Pressure") {
        //sensorWidget->setSensorData(identifier, sensorType, timestamp, "Tire Pressure", value);
        //valueName = "Tire Pressure";
    //} else if (sensorType == "Brake Temperature") {
        //sensorWidget->setSensorData(identifier, sensorType, timestamp, "Brake Temperature", value);
        //valueName = "Brake Temperature";
    //} else {
        //qWarning() << "Undefined sensor type: " << sensorType;
        //delete sensorWidget;
        //return;
    //}

    // Sets CSS style
    //QString style;
    //if (sensorType == "Fuel Flow") {
        //style = "background-color: #a91b16; color: white;";
    //} else if (sensorType == "Tire Pressure") {
        //style = "background-color: #d73c37; color: white;";
    //} else if (sensorType == "Brake Temperature") {
        //style = "background-color: #f76f6b; color: white;";
    //} else {

        //style = "background-color: gray; color: black;";
    //}
    //sensorWidget->setStyle(style);


    // Add sensor widget to sensor panel
    //QVBoxLayout* sensorPanelLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
    //if (sensorPanelLayout) {
        //sensorPanelLayout->addWidget(sensorWidget);
        // Connect the clicked signal of the newly added sensorWidget to highlightSelectedWidget, and updateChart

        //connect(sensorWidget, &SensorWidget::sensorClicked, this, &MainWindow::highlightSelectedWidget);
        //connect(sensorWidget, &SensorWidget::updateChartRequested, this, &MainWindow::updateChart);



    //} else {
        //qWarning() << "Could not access to sensorPanel layout.";
        //delete sensorWidget;
    //}

    //m_jsonController.addSensorData(identifier, sensorType, timestamp, valueName, value);



//}

void MainWindow::addSensorToPanel(const QString& sensorType, int identifier, const QString& timestamp, double value) {
    // Creates a new sensor widget with fixed size
    SensorWidget* sensorWidget = new SensorWidget();
    sensorWidget->setFixedSize(210, 150);

    // Create sensor object
    Sensor::AbstractSensor* sensor = nullptr;
    QDateTime qTimestamp = QDateTime::fromString(timestamp, Qt::ISODate);

    if (sensorType == "Fuel Flow") {
        sensor = new Sensor::FuelFlowSensor(identifier, sensorType.toStdString(), value, qTimestamp);
    } else if (sensorType == "Tire Pressure") {
        sensor = new Sensor::TirePressureSensor(identifier, sensorType.toStdString(), value, qTimestamp);
    } else if (sensorType == "Brake Temperature") {
        sensor = new Sensor::BrakeTemperatureSensor(identifier, sensorType.toStdString(), value, qTimestamp);
    } else {
        qWarning() << "Undefined sensor type: " << sensorType;
        delete sensorWidget;
        return;
    }

    // Sets sensor data
    sensorWidget->setSensorData(sensor);

    // Sets CSS style
    QString style;
    if (sensorType == "Fuel Flow") {
        style = "background-color: #a91b16; color: white;";
    } else if (sensorType == "Tire Pressure") {
        style = "background-color: #d73c37; color: white;";
    } else if (sensorType == "Brake Temperature") {
        style = "background-color: #f76f6b; color: white;";
    } else {
        style = "background-color: gray; color: black;";
    }
    sensorWidget->setStyle(style);

    // Add sensor widget to sensor panel
    QVBoxLayout* sensorPanelLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
    if (sensorPanelLayout) {
        sensorPanelLayout->addWidget(sensorWidget);
        // Connect the clicked signal of the newly added sensorWidget to highlightSelectedWidget, and updateChart
        connect(sensorWidget, &SensorWidget::sensorClicked, this, &MainWindow::highlightSelectedWidget);
        connect(sensorWidget, &SensorWidget::updateChartRequested, this, &MainWindow::updateChart);
    } else {
        qWarning() << "Could not access to sensorPanel layout.";
        delete sensorWidget;
    }

    // Add sensor data to JSON controller
    m_jsonController.addSensorData(sensor);
}

//void MainWindow::editSensor() {

    //QDialog dialog(this);
    //dialog.setWindowTitle("Edit Sensor");
    //QVBoxLayout layout(&dialog);

    //QLabel label("Enter Sensor Identifier:", &dialog);
    //QLineEdit identifierLineEdit(&dialog);
    //QPushButton okButton("OK", &dialog);
    //layout.addWidget(&label);
    //layout.addWidget(&identifierLineEdit);
    //layout.addWidget(&okButton);


    //connect(&okButton, &QPushButton::clicked, &dialog, [&](){
        //int identifier = identifierLineEdit.text().toInt();


        //QVBoxLayout* sensorPanelLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
        //if (!sensorPanelLayout) {
            //qWarning() << "Could not access sensor panel layout.";
            //return;
        //}

        //SensorWidget* targetSensor = nullptr;


        //for (int i = 0; i < sensorPanelLayout->count(); ++i) {
            //SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(sensorPanelLayout->itemAt(i)->widget());
            //if (!sensorWidget) {
                //continue;
            //}


           // if (sensorWidget->getIdentifier() == identifier) {
               // targetSensor = sensorWidget;
               // break;
           // }
       // }


        //if (targetSensor) {
            //QDialog editDialog(this);
           // editDialog.setWindowTitle("Edit Sensor Data");
           // QVBoxLayout editLayout(&editDialog);


           // QComboBox typeComboBox(&editDialog);
           // typeComboBox.addItem("Fuel Flow");
           // typeComboBox.addItem("Tire Pressure");
           // typeComboBox.addItem("Brake Temperature");

           // QLineEdit timestampLineEdit(targetSensor->getTimestamp(), &editDialog);
           // QLineEdit valueLineEdit(QString::number(targetSensor->getNumericData()), &editDialog);


           // editLayout.addWidget(&typeComboBox);
           // editLayout.addWidget(&timestampLineEdit);
           // editLayout.addWidget(&valueLineEdit);

           // QPushButton saveButton("Save", &editDialog);
           // editLayout.addWidget(&saveButton);


            //connect(&saveButton, &QPushButton::clicked, &editDialog, [&](){
               // QString sensorType = typeComboBox.currentText();
               // QString timestamp = timestampLineEdit.text();
               // double value = valueLineEdit.text().toDouble();


                //targetSensor->setSensorData(targetSensor->getIdentifier(), sensorType, timestamp, targetSensor->getValueName(), value);


                //jsonController->updateSensorData(targetSensor->getIdentifier(), sensorType, timestamp, value, targetSensor->getValueName());

                //statusBar()->showMessage("Sensor data updated successfully!", 3000);


                //editDialog.close();
            //});

            //editDialog.exec();
       // } else {
           // QMessageBox::critical(&dialog, "Error", "Sensor with specified identifier not found.");
       // }


       // dialog.close();
   // });

   // dialog.exec();
//}

void MainWindow::editSensor() {
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Sensor");
    QVBoxLayout layout(&dialog);

    QLabel label("Enter Sensor Identifier:", &dialog);
    QLineEdit identifierLineEdit(&dialog);
    QPushButton okButton("OK", &dialog);
    layout.addWidget(&label);
    layout.addWidget(&identifierLineEdit);
    layout.addWidget(&okButton);

    connect(&okButton, &QPushButton::clicked, &dialog, [&](){
        int identifier = identifierLineEdit.text().toInt();

        QVBoxLayout* sensorPanelLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
        if (!sensorPanelLayout) {
            qWarning() << "Could not access sensor panel layout.";
            return;
        }

        SensorWidget* targetSensor = nullptr;

        for (int i = 0; i < sensorPanelLayout->count(); ++i) {
            SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(sensorPanelLayout->itemAt(i)->widget());
            if (!sensorWidget) {
                continue;
            }

            if (sensorWidget->getIdentifier() == identifier) {
                targetSensor = sensorWidget;
                break;
            }
        }

        if (targetSensor) {
            QDialog editDialog(this);
            editDialog.setWindowTitle("Edit Sensor Data");
            QVBoxLayout editLayout(&editDialog);

            QComboBox typeComboBox(&editDialog);
            typeComboBox.addItem("Fuel Flow");
            typeComboBox.addItem("Tire Pressure");
            typeComboBox.addItem("Brake Temperature");

            QLineEdit timestampLineEdit(targetSensor->getTimestamp(), &editDialog);
            QLineEdit valueLineEdit(QString::number(targetSensor->getNumericData()), &editDialog);

            editLayout.addWidget(&typeComboBox);
            editLayout.addWidget(&timestampLineEdit);
            editLayout.addWidget(&valueLineEdit);
            QPushButton saveButton("Save", &editDialog);
            editLayout.addWidget(&saveButton);

            connect(&saveButton, &QPushButton::clicked, &editDialog, [&](){
                QString sensorType = typeComboBox.currentText();
                QString timestamp = timestampLineEdit.text();
                double value = valueLineEdit.text().toDouble();

                // Crea un nuovo oggetto sensore con i dati aggiornati
                Sensor::AbstractSensor* updatedSensor = nullptr;
                QDateTime qTimestamp = QDateTime::fromString(timestamp, Qt::ISODate);

                if (sensorType == "Fuel Flow") {
                    updatedSensor = new Sensor::FuelFlowSensor(identifier, sensorType.toStdString(), value, qTimestamp);
                } else if (sensorType == "Tire Pressure") {
                    updatedSensor = new Sensor::TirePressureSensor(identifier, sensorType.toStdString(), value, qTimestamp);
                } else if (sensorType == "Brake Temperature") {
                    updatedSensor = new Sensor::BrakeTemperatureSensor(identifier, sensorType.toStdString(), value, qTimestamp);
                }

                if (updatedSensor) {
                    targetSensor->setSensorData(updatedSensor);
                    m_jsonController.updateSensorData(updatedSensor);

                    statusBar()->showMessage("Sensor data updated successfully!", 3000);
                    editDialog.close();
                } else {
                    QMessageBox::critical(&editDialog, "Error", "Failed to create updated sensor.");
                }
            });

            editDialog.exec();
        } else {
            QMessageBox::critical(&dialog, "Error", "Sensor with specified identifier not found.");
        }

        dialog.close();
    });

    dialog.exec();
}

void MainWindow::deleteSensor() {
    QDialog dialog(this);
    dialog.setWindowTitle("Delete Sensor");
    QVBoxLayout layout(&dialog);

    QLabel label("Enter Sensor Identifier to Delete:", &dialog);
    QLineEdit identifierLineEdit(&dialog);
    QPushButton deleteButton("Delete", &dialog);
    layout.addWidget(&label);
    layout.addWidget(&identifierLineEdit);
    layout.addWidget(&deleteButton);

    connect(&deleteButton, &QPushButton::clicked, &dialog, [&](){
        int identifier = identifierLineEdit.text().toInt();

        QVBoxLayout* sensorPanelLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
        if (!sensorPanelLayout) {
            qWarning() << "Could not access sensor panel layout.";
            return;
        }

        SensorWidget* targetSensor = nullptr;

        for (int i = 0; i < sensorPanelLayout->count(); ++i) {
            SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(sensorPanelLayout->itemAt(i)->widget());
            if (!sensorWidget) {
                continue;
            }

            if (sensorWidget->getIdentifier() == identifier) {
                targetSensor = sensorWidget;

                // Delete sensor widget from sensor panel
                sensorPanelLayout->removeWidget(targetSensor);
                delete targetSensor;
                targetSensor = nullptr;

                // Add sensor identifier to deleted sensor identifiers array
                deletedSensorIds.append(identifier);

                // Confirmation message
                statusBar()->showMessage("Sensor deleted successfully!", 3000);

                dialog.close();
                break;
            }
        }

        if (!targetSensor) {
            return;
        }
    });

    dialog.exec();
}


//QMap<QString, QVector<QPair<QDateTime, double>>> MainWindow::collectSensorData() {
    //QMap<QString, QVector<QPair<QDateTime, double>>> sensorData;

    // Visits all sensor widgets to collect data
    //QWidget* scrollWidget = sensorPanelScrollArea->widget();
    //if (scrollWidget) {
       // QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollWidget->layout());
        //if (layout) {
            //for (int i = 0; i < layout->count(); ++i) {
                //SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(layout->itemAt(i)->widget());
                //if (sensorWidget) {
                   // QString type = sensorWidget->getType();
                   // QString timestampString = sensorWidget->getTimestamp();
                   // QDateTime timestamp = QDateTime::fromString(timestampString, "yyyy-MM-dd HH:mm:ss");
                   // double value = sensorWidget->getNumericData();

                    // Check if the sensor type already exists in the map
                    //if (!sensorData.contains(type)) {
                        // If not, create a new entry for the sensor type
                        //sensorData[type] = QVector<QPair<QDateTime, double>>();
                    //}

                    // Append the sensor data to the corresponding type
                    //sensorData[type].append(qMakePair(timestamp, value));
                //}
            //}
        //}
   // }

   // return sensorData;
//}

QMap<QString, QVector<QPair<QDateTime, double>>> MainWindow::collectSensorData() {
    QMap<QString, QVector<QPair<QDateTime, double>>> sensorData;

    // Visits all sensor widgets to collect data
    QWidget* scrollWidget = sensorPanelScrollArea->widget();
    if (scrollWidget) {
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(scrollWidget->layout());
        if (layout) {
            for (int i = 0; i < layout->count(); ++i) {
                SensorWidget* sensorWidget = qobject_cast<SensorWidget*>(layout->itemAt(i)->widget());
                if (sensorWidget) {
                    // Get sensor data
                    Sensor::AbstractSensor* sensor = sensorWidget->getSensor();
                    QString type = QString::fromStdString(sensor->getType());
                    QDateTime timestamp = sensor->getTimestamp();
                    double value = sensorWidget->getNumericData();

                    // Check if the sensor type already exists in the map
                    if (!sensorData.contains(type)) {
                        // If not, create a new entry for the sensor type
                        sensorData[type] = QVector<QPair<QDateTime, double>>();
                    }

                    // Append the sensor data to the corresponding type
                    sensorData[type].append(qMakePair(timestamp, value));
                }
            }
        }
    }

    return sensorData;
}


void MainWindow::displaySensorData(QChart *chart, const QString& selectedSensorType) {
    // Collect sensor data
    QMap<QString, QVector<QPair<QDateTime, double>>> sensorData = collectSensorData();

    // Clear previous series
    chart->removeAllSeries();

    // Configure X axis
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");
    axisX->setTitleText("Timestamp");
    axisX->setLabelsAngle(-45);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Configure Y axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(5);
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);

    // Add line series and scatter series for the selected sensor type
    if (sensorData.contains(selectedSensorType)) {
        const QVector<QPair<QDateTime, double>>& data = sensorData.value(selectedSensorType);

        // Create line series
        QLineSeries *lineSeries = new QLineSeries;
        lineSeries->setName(selectedSensorType);

        // Create scatter series
        QScatterSeries *scatterSeries = new QScatterSeries;
        scatterSeries->setName(selectedSensorType + " Data Points");

        // Add data points to series
        for (const auto& pair : data) {
            qint64 timestamp = pair.first.toMSecsSinceEpoch();
            double value = pair.second;
            lineSeries->append(timestamp, value);
            scatterSeries->append(timestamp, value);
        }

        // Add series to chart
        chart->addSeries(lineSeries);
        chart->addSeries(scatterSeries);

        // Attach series to axes
        lineSeries->attachAxis(axisX);
        lineSeries->attachAxis(axisY);
        scatterSeries->attachAxis(axisX);
        scatterSeries->attachAxis(axisY);
    }

    // Update chart view
    m_chartView->setChart(chart);
}


void MainWindow::updateChart(const QString& selectedSensorType) {
    // Create chart
    QChart *chart = new QChart();

    // Display data for the selected sensor type
    displaySensorData(chart, selectedSensorType);

    // Update the chart view
    m_chartView->setChart(chart);
}
void MainWindow::highlightSelectedWidget(SensorWidget* selectedWidget) {



    QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(sensorPanelScrollArea->widget()->layout());
    if (!scrollLayout) {
        qWarning() << "Could not access scroll area layout.";
        return;
    }


    for (int i = 0; i < scrollLayout->count(); ++i) {
        SensorWidget* widget = qobject_cast<SensorWidget*>(scrollLayout->itemAt(i)->widget());
        if (widget) {

            QString existingStyle = widget->styleSheet();
            QRegularExpression borderRegex("border.*");
            existingStyle.remove(borderRegex);
            widget->setStyleSheet(existingStyle);
        }
    }

    // Adds selected style
    if (selectedWidget) {
        // Adds border if sensor is selected
        selectedWidget->setStyleSheet(selectedWidget->styleSheet() + "border: 3px solid white;");
    }

}


