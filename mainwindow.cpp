#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

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

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::toStandBy);

    m_timer_status = new QTimer(this);
    connect(m_timer_status, &QTimer::timeout, this, &MainWindow::resetStatus);

    connectWidgets();
    m_machine->getDevicecStatus();
}

void MainWindow::connectWidgets()
{
    connect(&m_stanbyscreen, SIGNAL(exit()), this, SLOT(exit()));
    connect(&m_stanbyscreen, SIGNAL(rescan()), this, SLOT(rescan()));

    connect(&m_scanscreen, SIGNAL(backToStandBy()), this, SLOT(toStandBy()));
    connect(&m_scanscreen, SIGNAL(getCalendar()), this, SLOT(onGetCalendar()));

    connect(m_machine, &Machine::deviceStatusChanged, this, &MainWindow::onDeviceStatusChanged);
    connect(m_machine, &Machine::scanSuccess, this, &MainWindow::onScanSuccess);
    connect(m_machine, &Machine::scanFail, this, &MainWindow::onScanFail);
    connect(m_machine, &Machine::updateStatusBar, this, &MainWindow::setStatusBarText);
    connect(m_machine, &Machine::receivedCalendar, this, &MainWindow::onReceivedCalendar);
    connect(m_machine, &Machine::receivedCalendarFail, this, &MainWindow::onReceivedCalendarFail);
    connect(m_machine, &Machine::receivedSetting, this, &MainWindow::onReceivedSetting);
    connect(m_machine, &Machine::receivedSettingFail, this, &MainWindow::onReceivedSettingFail);
}

void MainWindow::exit()
{
    this->close();
    m_machine->close();
}

void MainWindow::rescan()
{
    m_machine->rescan();
}

void MainWindow::onDeviceStatusChanged(QStringList data)
{
    m_stanbyscreen.setDevices(data);
}

void MainWindow::setStatusBarText(QString text, QString rgb)
{
    ui->statusbar->showMessage(text);
    ui->statusbar->setStyleSheet("background-color: "+rgb+";");
    m_timer_status->start(2000);
}

void MainWindow::resetStatus()
{
    ui->statusbar->showMessage("Waiting");
    ui->statusbar->setStyleSheet("background-color: rgb(000,255,0);");
    m_timer_status->stop();
}

void MainWindow::onScanSuccess(QString name, QString time, QString type, QString userid)
{
    m_scanscreen.setNameLabel(name);
    QDateTime datetime = QDateTime::fromString(time, Qt::ISODate);
    if(type == "Checkin"){
        setStyleSheet("background-color: "+m_checkin_color);
        m_scanscreen.setIsCheckin(true);
    }else{
        setStyleSheet("background-color: "+m_checkout_color);
        m_scanscreen.setIsCheckin(false);
    }
    m_scanscreen.setTimeLabel(type + ": " + datetime.toString("dd.MM.yyyy HH:mm"));
    m_scanscreen.hideCalendar();
    m_current_user_id=userid;
    ui->stackedWidget->setCurrentIndex(1);
    setStatusBarText("Success...", "rgb(135,206,250)");
    m_timer->start(5000);
}

void MainWindow::toStandBy()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->statusbar->showMessage("Waiting...");
    ui->statusbar->setStyleSheet("background-color: rgb(0, 255, 0);");
    setStyleSheet("background-color: 'white'");
    m_timer->stop();
}

void MainWindow::onScanFail(QString error)
{
    setStatusBarText("Error: " +error, "rgb(255, 0, 0)");
    m_stanbyscreen.setMessageError(error);
}

void MainWindow::runMachine()
{
    m_machine->startScan();
}

void MainWindow::onGetCalendar()
{
    m_timer->stop();
    m_machine->getCalendar(m_current_user_id);
}

void MainWindow::onReceivedCalendar(QJsonArray jsonArray)
{
    m_scanscreen.setItems(jsonArray);
    m_scanscreen.showCalendar();
    setStatusBarText("Success...", "rgb(135,206,250)");
    //m_timer->start(10000);
}

void MainWindow::onReceivedCalendarFail(QString error)
{
    setStatusBarText("Error: " + error, "rgb(255,000,000)");
}

void MainWindow::onReceivedSetting(QJsonObject jsonObj)
{
//    ["businessName"].toObject()["settingValue"];
    m_checkin_color = jsonObj["checkInColor"].toObject()["settingValue"].toString();
    m_checkout_color = jsonObj["checkOutColor"].toObject()["settingValue"].toString();
    business_name = jsonObj["businessName"].toObject()["settingValue"].toString();
    m_stanbyscreen.setLogoUrl(jsonObj["logo"].toObject()["settingValue"].toString());
    m_scanscreen.setSettings(jsonObj["logo"].toObject()["settingValue"].toString(), jsonObj["checkInColor"].toObject()["settingValue"].toString(), jsonObj["checkOutColor"].toObject()["settingValue"].toString());
}

void MainWindow::onReceivedSettingFail(QString error)
{

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_machine;
}

