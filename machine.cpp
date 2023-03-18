#include "machine.h"
#include "rfid.h"
#include "qdebug.h"
#include <QFile>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>


Machine::Machine(QObject* parent) : QObject(parent)
{
    IScanDevice* rfid = new RFID();
    m_scanDevices.append(rfid);
    connectDevices();
    connectServer();
    startScan();
    updateDeviceStatus();
}

void Machine::connectDevices()
{
    for(auto scanDevice :  m_scanDevices)
    {
        connect(scanDevice, &IScanDevice::connected, this, &Machine::connected);
        connect(scanDevice, &IScanDevice::disconnected, this, &Machine::disconnected);
        connect(scanDevice, &IScanDevice::scanResult, this, &Machine::scanResult);
    }
}

void Machine::updateDeviceStatus()
{
    QStringList data;
    for(auto scanDevice : m_scanDevices)
    {
        QString row = scanDevice->getType() + " : " + (scanDevice->isOk() ? "OK" : "ERROR");
        data.append(row);
    }
    emit deviceStatusChanged(data);
}

void Machine::connectServer()
{
    QFile file(m_license_path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", file.errorString());
        m_status = 2;
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList fields = line.split(";");
    m_url_server = fields.at(0);
    m_device_token = fields.at(1);
    qDebug() << m_device_token;
    file.close();

    // call api to get general settings: logo, success color, failed color
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl url(m_url_server+"/settings");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = mgr->post(request, data);
    qDebug() << "called api";
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            if(obj["code"].toDouble() == 0)
            {
                qDebug() << obj["data"].toObject()["businessName"].toObject()["settingValue"];
                emit receivedSetting(obj["data"].toObject());
            }
            else
            {
                qDebug() << obj["message"];
                emit receivedSettingFail(obj["message"].toString());
            }
        }
    });
}

Machine::~Machine()
{

}

void Machine::getCalendar(QString user_id)
{
    qDebug() << "user_id: " << user_id;
    emit updateStatusBar("Loading calendar...", "rgb(255,140,0)");

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl url(m_url_server+"/calendar");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["userId"] = user_id;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = mgr->post(request, data);

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            if(obj["code"].toDouble() == 0)
            {
                QJsonArray jsonArray = obj["data"].toArray();
                emit receivedCalendar(jsonArray);
            }
            else
            {
                qDebug() << obj["message"];
                emit receivedCalendarFail(obj["message"].toString());
            }
        }
    });


}

void Machine::getDevicecStatus()
{
    updateDeviceStatus();
}

void Machine::close()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->closeport();
    }
}

void Machine::startScan()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->start();
    }
}

void Machine::rescan()
{
    for(auto scanDevice :  m_scanDevices)
    {
        scanDevice->setRescan();
    }
}

void Machine::connected(QString type)
{
    qDebug() << "connected: " + type;
    emit updateStatusBar(type + " is connected!", "rgb(000,255,000)");
    updateDeviceStatus();
}
void Machine::disconnected(QString type)
{
    qDebug() << "disconnected: " + type;
    emit updateStatusBar(type + " is disconnected!", "rgb(255,000,000)");
    updateDeviceStatus();
}

void Machine::scanResult(QString id, QString type)
{
    emit updateStatusBar("Loading...", "rgb(255,140,0)");
    qDebug() << "scanResult: " + id + " - " + type;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl url(m_url_server);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QTime time = QTime::currentTime();
    QString formattedTime = time.toString("hh:mm");
    QDate date = QDate::currentDate();
    QString dateStr = date.toString("yyyy-MM-dd");

    QJsonObject obj;
    obj["type"] = type;
    obj["cardNumber"] = id;
    obj["token"] = m_device_token;
    obj["datetime"] = dateStr + " " + formattedTime;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = mgr->post(request, data);

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();

            qDebug() << obj;
            if(obj["code"].toDouble() == 0)
            {
                QJsonValue val = obj.value(QString("data"));
                QJsonObject item = val.toObject();
                emit scanSuccess(item["name"].toString(), item["time"].toString(), item["type"].toString(), item["userId"].toString());
            }
            else
            {
                emit scanFail(obj["message"].toString());
            }

        }
        else{
            QString err = reply->errorString();
            qDebug() << err;
            emit scanFail(err);
        }
        reply->deleteLater();
    });


}
