#include "standbyscreen.h"
#include "ui_standbyscreen.h"
#include <QPixmap>
#include <QTime>
#include <QDate>
#include "qdebug.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSslConfiguration>

StandByScreen::StandByScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandByScreen)
{
    ui->setupUi(this);
    QPixmap pic(logi_url);
    ui->logo->setPixmap(pic);

    QDate date = QDate::currentDate();
    QString dateStr = date.toString("dd.MM.yyyy");
    ui->message->setText(dateStr);

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

    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';

    ui->time->setText(text);
}

void StandByScreen::resetMessage()
{
//    ui->message->setText("Please scan your card!");
//    ui->message->setStyleSheet("font-size:35px;color: rgb(0, 0, 0);");
//    m_timer_message->stop();
}

void StandByScreen::setMessageError(QString error)
{
//    ui->message->setText(error);
//    ui->message->setStyleSheet("font-size:35px;color: rgb(255, 0, 0);");
//    m_timer_message->start(2000);
}

void StandByScreen::on_closeWindow_clicked()
{
    int ret = QMessageBox::critical(this, tr("Shutdown device"),
                                   tr("Do you really want to shutdown device?"),
                                   QMessageBox::Ok
                                   , QMessageBox::Cancel);
    if(ret == QMessageBox::Ok)
        emit exit();
}

void StandByScreen::on_pushButton_clicked()
{
    emit rescan();
}

void StandByScreen::setLogoUrl(QString url)
{
    logi_url = url;
    qDebug() << logi_url;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl qurl(logi_url);
    QNetworkRequest request(qurl);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    QNetworkReply *reply = mgr->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QPixmap pic;
            pic.loadFromData(reply->readAll());
            ui->logo->setPixmap(pic);
        }
        else{
            QString err = reply->errorString();
            qDebug() << err;
        }
        reply->deleteLater();
    });


}
