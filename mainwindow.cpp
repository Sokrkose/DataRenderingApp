#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "datareader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    signalWidget = new Widget(this);
    setCentralWidget(signalWidget);

    std::vector<int> testSignal = {0, 1, 0, 1, 1, 0, 0, 1};
    signalWidget->setSignalData(testSignal, 100, "test.txt");

    // const QString input_file = "C:/Users/skoseogl/cernbox/Documents/Qt/DataRenderingApp/data/waveform-data.txt";

    // struct data parsedData = dataReader::read(input_file);

    // qDebug() << "Parsed" << parsedData.values.size() << "data points.";

    // std::vector<int> first10(parsedData.values.begin(), parsedData.values.begin() + std::min(10, (int)parsedData.values.size()));
    // signalWidget->setSignalData(first10, parsedData.timestep_ns);

    // signalWidget->setSignalData(parsedData.values, parsedData.timestep_ns, input_file);

}

MainWindow::~MainWindow()
{
    delete ui;
}

