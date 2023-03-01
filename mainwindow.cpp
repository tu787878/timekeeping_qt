#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);

    ui->stackedWidget->addWidget(&m_stanbyscreen);
    ui->stackedWidget->addWidget(&m_scanscreen);

    ui->statusbar->showMessage("Waiting...");
    ui->statusbar->setStyleSheet("background-color: rgb(0, 255, 0);");
    m_machine = new Machine();

    connectWidgets();
}

void MainWindow::connectWidgets()
{
    connect(&m_stanbyscreen, SIGNAL(exit()), this, SLOT(exit()));
    connect(&m_stanbyscreen, SIGNAL(rescan()), this, SLOT(rescan()));

    connect(m_machine, &Machine::deviceStatusChanged, this, &MainWindow::onDeviceStatusChanged);
    connect(m_machine, &Machine::scanStatusChanged, this, &MainWindow::onScanStatusChanged);
}

void MainWindow::exit()
{
    this->close();
}

void MainWindow::rescan()
{
    m_machine->rescan();
}

void MainWindow::onDeviceStatusChanged(QString type)
{

}

void MainWindow::onScanStatusChanged(QString id, QString type)
{

}

void MainWindow::runMachine()
{
    m_machine->startScan();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_machine;
}

