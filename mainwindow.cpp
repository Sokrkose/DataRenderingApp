#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "datareader.h"
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Main Window Constructor
    signalWidget = new Widget(this);

    // std::vector<int> testSignal = {0, 1, 0, 1, 1, 0, 0, 1};
    // signalWidget->setSignalData(testSignal, 100, "test.txt");

    // Wrap it in a scroll area
    const QString input_file = "C:/Users/skoseogl/cernbox/Documents/Qt/DataRenderingApp/data/waveform-data.txt";
    struct data parsedData = dataReader::read(input_file);
    signalWidget->setSignalData(parsedData.values, parsedData.timestep_ns, input_file);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(signalWidget);

    setCentralWidget(scrollArea);

    // Optionally scroll to a position if needed
    scrollArea->horizontalScrollBar()->setValue(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

