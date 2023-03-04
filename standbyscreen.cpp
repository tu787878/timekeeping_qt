#include "standbyscreen.h"
#include "ui_standbyscreen.h"
#include <QPixmap>
#include <QTime>
#include <QDate>

StandByScreen::StandByScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandByScreen)
{
    ui->setupUi(this);
    QPixmap pic("/home/pi/Downloads/SmallLogoBW.png");
    ui->logo->setPixmap(pic);

    ui->message->setText("Please scan your card!");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &StandByScreen::showTime);
    timer->start(1000);

    m_timer_message = new QTimer(this);
    connect(m_timer_message, &QTimer::timeout, this, &StandByScreen::resetMessage);
}

StandByScreen::~StandByScreen()
{
    delete ui;
}

void StandByScreen::setDevices(QStringList data)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(data);
}

void StandByScreen::showTime()
{
    QDate date = QDate::currentDate();
    QString dateStr = date.toString("dd.MM.yyyy");
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    text = dateStr + " " + text;
    ui->time->setText(text);
}

void StandByScreen::resetMessage()
{
    ui->message->setText("Please scan your card!");
    ui->message->setStyleSheet("color: rgb(0, 0, 0);");
    m_timer_message->stop();
}

void StandByScreen::setMessageError(QString error)
{
    ui->message->setText(error);
    ui->message->setStyleSheet("color: rgb(255, 0, 0);");
    m_timer_message->start(2000);
}

void StandByScreen::on_closeWindow_clicked()
{
    emit exit();
}

void StandByScreen::on_pushButton_clicked()
{
    emit rescan();
}
