#include "machine.h"
#include "rfid.h"
#include "qdebug.h"
#include <QFile>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>


Machine::Machine(QObject* parent) : QObject(parent)
{
    IScanDevice* rfid = new RFID();
    m_scanDevices.append(rfid);
    connectDevices();
    connectServer();
    startScan();
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
    file.close();
    // try to call api
}

Machine::~Machine()
{

}

void Machine::getCalendar(QString user_id)
{
    qDebug() << "user_id: " << user_id;
    emit receivedCalendar();
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
    emit deviceStatusChanged(type);
}
void Machine::disconnected(QString type)
{
    qDebug() << "disconnected: " + type;
    emit deviceStatusChanged(type);
}

void Machine::scanResult(QString id, QString type)
{
    emit updateStatusBar("Loading...", "rgb(255,140,0)");
    qDebug() << "scanResult: " + id + " - " + type;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl url(m_url_server);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["type"] = type;
    obj["cardNumber"] = id;
    obj["token"] = m_device_token;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = mgr->post(request, data);

    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            qDebug() << obj;
            emit scanSuccess("Van Tu Nguyen", "test", "test");
        }
        else{
            QString err = reply->errorString();
            qDebug() << err;
            emit scanFail(err);
        }
        reply->deleteLater();
    });


}
